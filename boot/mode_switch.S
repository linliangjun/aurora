/**
 * 模式切换（16 位实地址模式 <--> 32 位保护模式）
 *
 * Copyright 2024 linliangjun
 */

#include "seg.h"

#define CODE32_DESC_SELE SEG_DESC_SELE(1, 0, 0)
#define DATA32_DESC_SELE SEG_DESC_SELE(2, 0, 0)

    .globl      real_to_prot
    .text

/**
 * 从 16 位实地址模式切换到 32 位保护模式（平坦内存模型、关闭中断）
 *
 * 注意：返回地址必须在 [0, 0x10000)
 */
real_to_prot:
    .code16
    push        %bp
    mov         %sp, %bp

    // 关闭中断
    cli

    // 加载 GDT
    pushl       $gdt
    push        $GDT_LEN - 1
    lgdt        -6(%bp)
    add         $6, %sp

    // 开启保护模式，CR0 寄存器的第 0 位置 1
    mov         %cr0, %eax
    or          $1, %eax
    mov         %eax, %cr0

    // 目前处于 16 位保护模式
    // 开启 A20 地址线（fast A20）
    in          $0x92, %al
    or          $2, %al
    out         %al, $0x92

    // 通过段间转移，让 cs 为 32 位的代码段描述符选择子
    // 刷新描述符高速缓冲寄存器和指令流水线
    jmp         $CODE32_DESC_SELE, $1f
1:
    .code32
    // 目前处于 32 位保护模式
    // 重新设置段寄存器
    mov         $DATA32_DESC_SELE, %ax
    mov         %ax, %ds
    mov         %ax, %es
    mov         %ax, %fs
    mov         %ax, %gs
    mov         %ax, %ss

    // "返回"
    mov         %bp, %sp
    pop         %bp
    xor         %edx, %edx
    pop         %dx
    jmp         *%edx


    .section    .rodata
    .align      8
gdt:
    SEG_DESC_NULL

    // 平坦内存模型的代码段和数据段
    SEG_DESC(0, 0xfffff, SEG_DESC_ACCESS(1, 0, 1, SEG_DESC_TYPE_CODE), SEG_DESC_FLAGS(1, 1, 0))
    SEG_DESC(0, 0xfffff, SEG_DESC_ACCESS(1, 0, 1, SEG_DESC_TYPE_WRITEABLE_DATA), SEG_DESC_FLAGS(1, 1, 0))
    .set        GDT_LEN, (. - gdt)

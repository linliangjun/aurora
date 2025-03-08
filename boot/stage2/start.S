/**
 * 第二阶段引导代码
 *
 * 该代码会使 CPU 进入 32 位保护模式（平坦内存模型），然后调用 main 函数。
 *
 * Copyright 2025 linliangjun
 */

#include "seg.h"

#define CODE32_DESC_SELE SEG_DESC_SELE(1, 0, 0)
#define DATA32_DESC_SELE SEG_DESC_SELE(2, 0, 0)

    .globl      _start

    .section    .text.startup, "ax"
    .code16
_start:
    // 关闭中断
    cli

    // 加载 GDT
    mov         %sp, %bp
    pushl       $gdt
    push        $GDT_LEN - 1
    lgdt        -6(%bp)
    add         $6, %sp

    // 开启保护模式，CR0 寄存器的第 0 位置 1
    mov         %cr0, %eax
    or          $1, %eax
    mov         %eax, %cr0

    /* 目前处于 16 位保护模式 */

    // 开启 A20 地址线（fast A20）
    in          $0x92, %al
    or          $2, %al
    out         %al, $0x92

    // 通过段间转移，让 cs 为 32 位的代码段描述符选择子
    // 刷新描述符高速缓冲寄存器和指令流水线
    jmp         $CODE32_DESC_SELE, $1f

    .code32
1:
    /* 目前处于 32 位保护模式 */

    // 重新设置段寄存器
    mov         $DATA32_DESC_SELE, %ax
    mov         %ax, %ds
    mov         %ax, %es
    mov         %ax, %fs
    mov         %ax, %gs
    mov         %ax, %ss

    // 设置栈顶（弹出驱动器号，并保存在 dx 中）
    and         $0xffff, %esp
    pop         %dx

    // 调用 main 函数
    and         $0xff, %edx
    push        %edx
    call        main

    .section    .rodata
    .align      8
gdt:
    SEG_DESC_NULL

    // 平坦内存模型的代码段和数据段
    SEG_DESC(0, 0xfffff, SEG_DESC_ACCESS(1, 0, 1, SEG_DESC_TYPE_CODE), SEG_DESC_FLAGS(1, 1, 0))
    SEG_DESC(0, 0xfffff, SEG_DESC_ACCESS(1, 0, 1, SEG_DESC_TYPE_WRITEABLE_DATA), SEG_DESC_FLAGS(1, 1, 0))
    .set        GDT_LEN, (. - gdt)

/**
 * CPU 模式切换（16 位实地址模式 <--> 32 位保护模式）
 *
 * Copyright 2025 linliangjun
 */

#include "aurora/boot/boot.h"
#include "aurora/seg.h"

#define CODE32_DESC_SELE SEG_DESC_SELE(1, 0, 0)
#define DATA32_DESC_SELE SEG_DESC_SELE(2, 0, 0)
#define CODE16_DESC_SELE SEG_DESC_SELE(3, 0, 0)
#define DATA16_DESC_SELE SEG_DESC_SELE(4, 0, 0)

    .globl      real_to_prot, prot_to_real
    .text

/**
 * 从 16 位实地址模式切换到 32 位保护模式（平坦内存模型、关闭中断）
 *
 * 示例：
 *     .code16
 *     push     $4              // 拷贝的栈大小，2 字节大小
 *     call     real_to_prot
 *     .code32
 *     // 32 位保护模式下的指令
 */
real_to_prot:
    .code16
    // 关闭中断
    cli

    // 保存现在的栈顶位置到 edx 中
    xor         %edx, %edx
    mov         %sp, %dx

    // 加载 GDT
    lgdt        gdt_desc

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
1:
    .code32
    /* 目前处于 32 位保护模式 */

    // 重新设置段寄存器
    mov         $DATA32_DESC_SELE, %ax
    mov         %ax, %ds
    mov         %ax, %es
    mov         %ax, %fs
    mov         %ax, %gs
    mov         %ax, %ss

    // 设置保护模式栈
    mov         prot_stack, %esp
    mov         prot_base_stack, %ebp

    // 解析出需要拷贝的栈大小，保存在 ecx 中
    xor         %ecx, %ecx
    mov         2(%edx), %cx

    // 如果不需要拷贝，则跳过
    cmp         $0, %ecx
    je          2f

    // 分配栈空间
    sub         %ecx, %esp

    // 拷贝原有的栈数据
    mov         %esp, %eax
    push        %edx
    push        %ecx
    add         $4, %edx
    push        %edx
    push        %eax
    call        memcpy
    add         $12, %esp
    pop         %edx

2:
    // 解析出返回地址，保存在 ax 中
    mov         (%edx), %ax

    // 返回
    jmp         *%ax


/**
 * 从 32 位保护模式切换到 16 位实地址模式（段寄存器固定为 0、开启中断）
 *
 * 示例：
 *     .code32
 *     push     $4              // 拷贝的栈大小，4 字节大小，只有最低 2 个字节有效
 *     call     prot_to_real
 * 1:
 *     .code16
 *     // 16 位实地址模式下的指令
 */
prot_to_real:
    .code32
    // 关闭中断
    cli

    // 保存保护模式栈
    mov         %esp, prot_stack
    addl        $8, prot_stack
    mov         %ebp, prot_base_stack

    // 解析出需要拷贝的栈大小，保存在 ecx 中
    xor         %ecx, %ecx
    mov         4(%esp), %cx

    // 如果不需要拷贝，则跳过
    cmp         $0, %ecx
    je          1f

    push        %ecx

    // 拷贝栈数据到 16 位实地址模式栈
    push        %ecx
    push        prot_stack
    mov         $REAL_STACK_INIT, %eax
    sub         %ecx, %eax
    push        %eax
    call        memcpy
    add         $12, %esp

    pop         %ecx

1:
    // 解析出返回地址，保存在 dx 中
    mov         (%esp), %dx

    // 设置 16 位保护模式下的段寄存器
    mov         $DATA16_DESC_SELE, %ax
    mov         %ax, %ds
    mov         %ax, %es
    mov         %ax, %ss

    // 通过段间转移，让 cs 为 16 位的代码段描述符选择子
    // 刷新描述符高速缓冲寄存器和指令流水线
    jmp         $CODE16_DESC_SELE, $2f
2:
    .code16
    /* 目前处于 16 位保护模式 */

    // 关闭 A20 地址线（fast A20）
    in          $0x92, %al
    and         $0xfd, %al
    out         %al, $0x92

    // 关闭保护模式，CR0 寄存器的第 0 位置 0
    mov         %cr0, %eax
    and         $0xfffffffe, %eax
    mov         %eax, %cr0

    /* 目前处于实地址模式 */
    jmp         $0, $3f
3:
    // 重新设置段寄存器
    xor         %ax, %ax
    mov         %ax, %ds
    mov         %ax, %es
    mov         %ax, %ss

    // 设置实模式栈
    mov         $REAL_STACK_INIT, %sp
    sub         %cx, %sp
    mov         %sp, %bp

    // 返回
    jmp         *%dx


    .data
prot_stack:
    .long       PROT_STACK_INIT
prot_base_stack:
    .long       PROT_STACK_INIT


    .section    .rodata
    .align      8
gdt:
    SEG_DESC_NULL

    // 平坦内存模型的代码段和数据段
    SEG_DESC(0, 0xfffff, SEG_DESC_ACCESS(1, 0, 1, SEG_DESC_TYPE_CODE), SEG_DESC_FLAGS(1, 1, 0))
    SEG_DESC(0, 0xfffff, SEG_DESC_ACCESS(1, 0, 1, SEG_DESC_TYPE_WRITEABLE_DATA), SEG_DESC_FLAGS(1, 1, 0))

    // 16 位保护模式的代码段和数据段
    SEG_DESC(0, 0xffff, SEG_DESC_ACCESS(1, 0, 1, SEG_DESC_TYPE_CODE), SEG_DESC_FLAGS(0, 0, 0))
    SEG_DESC(0, 0xffff, SEG_DESC_ACCESS(1, 0, 1, SEG_DESC_TYPE_WRITEABLE_DATA), SEG_DESC_FLAGS(0, 0, 0))
gdt_desc:
    .word       . - gdt - 1
    .long       gdt

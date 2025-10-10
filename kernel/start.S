/**
 * 内核入口
 *
 * Copyright 2025 linliangjun
 */

#include "kernel.h"

    .global	    _start

.section .hhk_init.text, "ax"
_start:
    // 将引导信息保存到 ebx 中
    pop         %ebx

    // 初始化高半核（设置页表并启用分页）
    call        hhk_init

    // 跳转到高地址内核入口
    mov         $_kernel_start, %eax
    jmp         *%eax

    .text
_kernel_start:

    // 初始化 GDT
    call        gdt_init

    // 设置段寄存器和内核栈
    mov         $DATA_SEG_SELE, %ax
    mov         %ax, %ds
    mov         %ax, %es
    mov         %ax, %fs
    mov         %ax, %gs
    mov         %ax, %ss
    mov         $kernel_stack, %esp
    mov         %esp, %ebp
    jmp         $CODE_SEG_SELE, $1f
1:
    push        %ebx
    call        main

    .bss
    .align      16
    .skip       KERNEL_STACK_SIZE
kernel_stack:

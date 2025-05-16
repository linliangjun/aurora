/**
 * 内核入口
 *
 * Copyright 2025 linliangjun
 */

#include "kernel.h"

    .global	    _start

    .text
_start:
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
    jmp         main

    .bss
    .align      16
    .skip       KERNEL_STACK_SIZE
kernel_stack:

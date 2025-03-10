/**
 * 内核入口
 *
 * Copyright 2025 linliangjun
 */

#define __ASSEMBLY__

#include "aurora/gdt.h"
#include "aurora/kernel.h"

    .text
    .global	    _start
_start:
    call        console_init
    call        gdt_init

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
    call        main

    .bss
    .align      16
    .skip       KERNEL_STACK_SIZE
kernel_stack:

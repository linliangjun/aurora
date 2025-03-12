/**
 * 内核入口
 *
 * Copyright 2025 linliangjun
 */

#define __ASSEMBLY__

#include "aurora/gdt.h"
#include "aurora/kernel.h"

    .section    .text.init, "ax"
    .global	    _start
_start:
    call        vmm_init
    jmp         kernel_entry

    .text
kernel_entry:
    // 将引导参数保存到 ebx 中
    pop         %ebx

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
    push        %ebx
    call        pmm_init
    add         $4, %esp

    call        main

    .bss
    .align      16
    .skip       KERNEL_STACK_SIZE
kernel_stack:

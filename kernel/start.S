/**
 * 内核入口
 *
 * Copyright 2024 linliangjun
 */

    .section    .text.startup, "ax"
    .global	    _start
_start:
    // 开启内存分页
    call        enable_mem_page
    jmp         kernel_start

    .text
kernel_start:
    call        console_init
    call        gdt_init
    call        mem_init
    add         $8, %esp
    mov         $stack_top, %esp

    call        main
    jmp         .

    .bss
    .align      16
    .skip       16 * 1024
stack_top:

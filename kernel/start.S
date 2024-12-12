/**
 * 内核入口
 *
 * Copyright 2024 linliangjun
 */

    .text
    .global	    _start
_start:
    pop         %eax
    mov         %al, ards_vec_size
    pop         %eax
    mov         %eax, ards_vec

    call        console_init
    call        gdt_init
    call        mem_init

    call        main
    jmp         .

    .bss
    .comm       ards_vec, 4
    .comm       ards_vec_size, 1

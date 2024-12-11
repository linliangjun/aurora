/**
 * 内核入口
 *
 * Copyright 2024 linliangjun
 */

    .section    .text.startup, "ax"
    .global	    _start
_start:
    call        console_init

    call        main
    jmp         .

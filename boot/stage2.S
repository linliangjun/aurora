/**
 * 第二阶段引导代码
 *
 * Copyright 2024 linliangjun
 */

    .globl      _start

    .section    .text.startup, "ax"
    .code16
_start:
    // 进入 32 位保护模式
    call        real_to_prot

    .code32
    // 调用 main
    xor         %edx, %edx
    pop         %dx
    push        %edx
    call        main
    add         $4, %esp

    jmp         .

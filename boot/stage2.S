/**
 * 第二阶段引导代码
 *
 * Copyright 2024 linliangjun
 */

#define KERNEL_ADDR 0x100000

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

    cmp         $0, %al
    jne         1f

    push        $ards_vec
    push        ards_vec_size
    jmp         KERNEL_ADDR

1:

    jmp         .

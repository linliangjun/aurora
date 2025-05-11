/**
 * BIOS 中断调用
 *
 * Copyright 2025 linliangjun
 */

    .globl      int_10h_0eh

    .text
int_10h_0eh:
    .code32
    push        %ebp
    mov         %esp, %ebp

    // 进入 16 位实地址模式
    push        $12
    call        prot_to_real
    .code16

    // 调用 BIOS 中断 int 0x10 0x0e
    mov         8(%bp), %al
    mov         $0xe, %ah
    int         $0x10

    // 进入 32 位保护模式
    push       $0
    call       real_to_prot
    .code32

    // 返回
    leave
    ret

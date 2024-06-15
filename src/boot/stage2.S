# BIOS Legacy 第二阶段引导代码
#
# Copyright 2024 linliangjun

    .extern putstrln_16, real2prot
    .globl  stage2_start

    .section .text.stage2
    .code16
stage2_start:
    # 打印字符串
    push    $7
    push    $message
    call    putstrln_16
    add     $4, %sp

    pushl   $0x7c00
    call    real2prot

    .code32
    movb     $'P', 0xb8000 + 160 * 2
    jmp     .

    .section .rodata.stage2
message:
    .string "Stage2 OK."

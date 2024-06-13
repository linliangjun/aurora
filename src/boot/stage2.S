# BIOS Legacy 第二阶段引导代码
#
# Copyright 2024 linliangjun

    .extern putstrln_16, panic_16
    .globl  stage2_start

    .section .text.stage2
    .code16
stage2_start:
    # 打印字符串
    push    $7
    push    $message
    call    putstrln_16
    add     $4, %sp

    # 到达流程末尾，panic
    push    $1                      # 错误码
    call    panic_16

    .section .rodata.stage2
message:
    .string "Stage2 OK."

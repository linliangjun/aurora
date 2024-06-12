# BIOS Legacy MBR 引导代码
#
# Copyright 2024 linliangjun

    .globl  start

    .text
    .code16
start:
    # 无限循环
    jmp     .

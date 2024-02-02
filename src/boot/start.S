# MBR
#
# Copyright 2024 linliangjun

    .text
    .code16
_start:
    # 设置段寄存器 cs
    jmp     $0, $begin              # cs = 0

begin:
    # 设置段寄存器 ds，es，ss
    xor     %ax, %ax
    mov     %ax, %ds
    mov     %ax, %es
    mov     %ax, %ss
    mov     $_start, %sp            # sp = 0x7c00

    # 设置显示模式为 80 * 25 彩色文本模式
    mov     $3, %ax
    int     $0x10

    # 打印字符串
    push    $1
    push    $7
    push    $prompt
    push    $0
    call    puts_16
    add     $8, %sp

    # 无限循环
    jmp     .

    .section .rodata
prompt:
    .string "Aurora OS is starting, just a moment."

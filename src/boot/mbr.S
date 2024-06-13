# BIOS Legacy MBR 引导代码
#
# Copyright 2024 linliangjun

    .extern setmode_16, putstrln_16
    .globl  start

    .section .text.start
    .code16
start:
    # 通过跳转间接设置段寄存器 cs
    jmp     $0, $begin              # cs = 0

begin:
    # 设置段寄存器 ds，es，ss
    xor     %ax, %ax
    mov     %ax, %ds                # ds = 0
    mov     %ax, %es                # es = 0
    mov     %ax, %ss                # ss = 0

    # 设置栈顶
    mov     $start, %sp             # sp = 0x7c00

    # 设置显示模式为 80 * 25 16 色文本模式
    push    $0
    push    $3
    call    setmode_16
    add     $4, %sp

    # 打印字符串
    push    $7
    push    $message
    call    putstrln_16
    add     $4, %sp

    # 无限循环
    jmp     .

    .section .rodata
message:
    .string "Hello world!"

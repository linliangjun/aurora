# BIOS Legacy MBR 引导代码
#
# Copyright 2024 linliangjun

    .globl  start

    .text
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
    mov     $3, %ax
    int     $0x10

    # 打印字符串
    mov     $0x1301, %ax
    mov     $7, %bx
    mov     len, %cx
    xor     %dx, %dx
    mov     $message, %bp
    int     $0x10

    # 无限循环
    jmp     .

    .section .rodata
message:
    .ascii  "Hello world!"
len:
    .word   . - message

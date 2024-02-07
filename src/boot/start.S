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

    # 保存驱动器号
    mov     %dl, driver_num

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

    # 加载 stage2
    mov     $0x211, %ax
    mov     $0x7e00, %bx
    mov     $2, %cx
    xor     %dh, %dh
    mov     driver_num, %dl
    int     $0x13

    # 跳转到 stage2 入口
    jmp     stage2_entry

    .section .rodata
prompt:
    .string "Aurora OS is starting, just a moment."

    .bss
    .comm   driver_num, 1

    .section .note.GNU-stack, "", @progbits

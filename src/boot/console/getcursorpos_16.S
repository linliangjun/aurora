# 获取光标位置函数
#
# uint8_t getcursorpos_16();
#
# 返回值：光标位置，高 8 位为行（从 0 开始），低 8 位为列（从 0 开始）
#
# Copyright 2024 linliangjun

    .globl  getcursorpos_16

    .text
    .code16
getcursorpos_16:
    push    %bp
    mov     %sp, %bp
    push    %bx
    mov     $3, %ah
    xor     %bh, %bh
    int     $0x10
    mov     %dx, %ax
    pop     %bx
    leave
    ret

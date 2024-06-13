# 设置显示模式
#
# void setmode_16(uint8_t mode, bool clear);
#
# 参数 mode: 显示模式
# 参数 clear: 是否清空屏幕
#
# Copyright 2024 linliangjun

    .globl  setmode_16

    .text
    .code16
setmode_16:
    push    %bp
    mov     %sp, %bp
    mov     4(%bp), %al
    mov     6(%bp), %ah
    cmp     $0, %ah
    je      1f
    add     $0x80, %al
1:
    xor     %ah, %ah
    int     $0x10
    leave
    ret

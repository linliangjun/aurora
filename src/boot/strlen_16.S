# 计算字符串长度函数
#
# uint16_t strlen_16(int8_t *str);
#
# 参数 str: 字符串首地址，注意：段寄存器为 es，由调用者设置
#
# 返回值：字符串长度
#
# Copyright 2024 linliangjun

    .globl  strlen_16

    .text
    .code16
strlen_16:
    push    %bp
    mov     %sp, %bp
    push    %bx
    push    %si

    mov     4(%bp), %bx
    xor     %si, %si
1:
    cmpb    $0, %es:(%bx, %si)
    je      2f
    inc     %si
    jmp     1b
2:
    mov     %si, %ax

    pop     %si
    pop     %bx
    leave
    ret

    .section .note.GNU-stack, "", @progbits

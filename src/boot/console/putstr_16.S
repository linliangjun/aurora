# 控制台打印字符串函数，支持转义字符 '\r'，'\n'
#
# void putstr_16(uint16_t pos, const int8_t *str, uint8_t attr, bool update_cursor);
#
# 参数 pos: 字符串显示的位置，高 8 位为行 [0, 25)，低 8 位为列 [0, 80)
# 参数 str: 字符串首地址，段寄存器为 es，由调用者设置
# 参数 attr: 字符属性
# 参数 update_cursor: 是否更新光标位置
#
# 注意：实际字符串长度不能超过 65535，否则会绕回导致无限循环。
#
# Copyright 2024 linliangjun

    .extern strlen_16
    .globl  putstr_16

    .text
    .code16
putstr_16:
    push    %bp
    mov     %sp, %bp

    # 获取字符串长度（这里没有立即平衡栈）
    push    6(%bp)
    call    strlen_16
    mov     %ax, %cx

    # 调用 BIOS 中断 0x10/0x1300 打印字符串
    push    %bx
    push    %bp
    mov     10(%bp), %ax
    and     $1, %ax
    add     $0x1300, %ax
    xor     %bh, %bh
    mov     8(%bp), %bl
    mov     4(%bp), %dx
    mov     6(%bp), %bp
    int     $0x10
    pop     %bp
    pop     %bx
    leave
    ret

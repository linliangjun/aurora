# 控制台打印字符串函数，支持转义字符 '\r'，'\n'
#
# void putstrln_16(const int8_t *str, uint8_t attr);
#
# 参数 str: 字符串首地址，段寄存器为 es，由调用者设置
# 参数 attr: 字符属性
#
# 注意：实际字符串长度不能超过 65535，否则会绕回导致无限循环。
#
# Copyright 2024 linliangjun

    .extern getcursorpos_16, putstr_16
    .globl  putstrln_16

    .text
    .code16
putstrln_16:
    push    %bp
    mov     %sp, %bp

    # 获取光标位置
    call    getcursorpos_16

    # 计算下一行
    cmp     $0, %al
    je      1f
    xor     %al, %al
    inc     %ah
1:
    # 打印字符串
    push    $1                      # update_cursor = true
    push    6(%bp)                  # attr
    push    4(%bp)                  # 字符串首地址
    push    %ax                     # 行（ah），列（al）
    call    putstr_16               # 调用 putstr_16 函数

    leave
    ret

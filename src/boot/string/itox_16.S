# 数字转 16 进制字符串函数
#
# int8_t *itox_16(uint16_t i);
#
# 参数 i：数字
#
# 返回值：16 进制字符串，最少 4 字节，由调用者创建，并将其指针作为第一个参数传入
#
# 注意：该函数不会设置字符串的终止符
#
# Copyright 2024 linliangjun

# REG  0x1234

    .globl  itox_16

    .text
    .code16
itox_16:
    push    %bp
    mov     %sp, %bp
    push    %bx
    mov     4(%bp), %bx
    mov     6(%bp), %ax
    push    %ax

    xchg    %ah, %al
    push    %ax
    call    btox_16
    add     $2, %sp
    mov     %ax, (%bx)

    call    btox_16
    add     $2, %sp
    mov     %ax, 2(%bx)

    pop     %bx
    leave
    ret

# int16_t btox_16(uint8_t i);
btox_16:
    push    %bp
    mov     %sp, %bp
    mov     4(%bp), %ax
    mov     %al, %ah

    mov     $2, %cx
1:
    xchg    %al, %ah
    cmp     $2, %cx
    je      2f
    shr     $4, %al
    jmp     3f
2:
    and     $0xf, %al
3:
    # 若小于 9，则加 48，否则加 87
    cmp     $9, %al
    jbe     4f
    add     $87, %al
    jmp     5f
4:
    add     $48, %al
5:
    loop    1b
    leave
    ret

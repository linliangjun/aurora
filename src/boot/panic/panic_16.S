# panic 函数，不会返回
#
# void panic_16(uint16_t errorcode);
#
# 参数 errorcode：错误码
#
# Copyright 2024 linliangjun

    .extern itox_16, putstrln_16, getcursorpos_16, putstr_16
    .globl  panic_16

    .text
    .code16
panic_16:
    push    %bp
    mov     %sp, %bp

    # 打印字符串
    push    $12
    push    $message
    call    putstrln_16
    add     $4, %sp

    # 将错误码转换为 16 进制字符串
    push    4(%bp)
    push    $errorcode
    call    itox_16
    add     $4, %sp

    # 获取光标位置
    call    getcursorpos_16

    # 打印错误码
    push    $1
    push    $12
    push    $errorcode
    push    %ax
    call    putstr_16
    add     $8, %sp

    # 无限循环
1:
    hlt
    jmp     1b

    .section .rodata
message:
    .string "BOOT PANIC! ECODE: 0x"

    .bss
    .lcomm  errorcode, 5

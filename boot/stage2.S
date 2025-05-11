/**
 * 第二阶段引导代码
 *
 * Copyright 2025 linliangjun
 */

#include "boot/boot.h"

    .globl      stage2_entry

    .text
    .code16
stage2_entry:
    // 打印消息
    TTY_WRITE(msg)

    jmp         .

    .section    .rodata
msg:
    .string     "The second stage is OK!\r\n"

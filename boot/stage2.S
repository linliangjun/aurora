/**
 * 第二阶段引导代码
 *
 * Copyright 2025 linliangjun
 */

#include "aurora/boot/boot.h"

    .globl      stage2_entry

    .text
    .code16
stage2_entry:
    // 打印消息
    TTY_WRITE_16(msg)

    // 进入 32 位保护模式
    push        $2
    call        real_to_prot

    .code32
    // 保存驱动器号到 dx 中
    pop         %dx

    // 调用 main 函数
    push        %edx
    call        main

    .section    .rodata
msg:
    .string     "The second stage is OK!\r\n"

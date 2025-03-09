/**
 * 第二阶段引导代码
 *
 * 该代码会使 CPU 进入 32 位保护模式（平坦内存模型），然后调用 main 函数。
 *
 * Copyright 2025 linliangjun
 */

    .globl      _start

    .section    .text.startup, "ax"
    .code16
_start:
    // 进入 32 位保护模式
    push        $2
    call        real_to_prot
    .code32
1:
    // 保存驱动器号到 dx 中
    pop         %dx

    // 调用 main 函数
    and         $0xff, %edx
    push        %edx
    call        main

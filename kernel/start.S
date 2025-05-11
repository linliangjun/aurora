/**
 * 内核入口
 *
 * Copyright 2025 linliangjun
 */

    .global	    _start

    .section    .text.startup, "ax"
_start:
    jmp         main

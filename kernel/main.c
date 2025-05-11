/**
 * 内核主函数
 *
 * Copyright 2025 linliangjun
 */

#include "tty.h"
#include "types.h"

__attribute__((noreturn)) void main(void)
{
    tty_write("The kernel is OK!\n");
    while (true)
        ;
}

/**
 * stage2 主函数
 *
 * Copyright 2024 linliangjun
 */

#include "aurora/console.h"

#include <stddef.h>

static const int8_t message[] = "Loading kernel...";

__attribute__((__noreturn__)) void stage2_main()
{
    console_write(console_cursor_get(), message, true, true);
    while (true)
        ;
}

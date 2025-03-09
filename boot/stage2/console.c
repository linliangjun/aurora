/**
 * 控制台驱动
 *
 * Copyright 2025 linliangjun
 */

#include "console.h"
#include "bios_call.h"

void console_write(const char *str)
{
    char c;
    while ((c = *str++))
        int_10h_0eh(c);
}

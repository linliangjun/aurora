/**
 * TTY 驱动
 *
 * Copyright 2025 linliangjun
 */

#include "tty.h"
#include "boot/bios_call.h"

void tty_write(const char *msg)
{
    char c;
    while ((c = *msg++))
        int_10h_0eh(c);
}

/**
 * 内核打印
 *
 * Copyright 2025 linliangjun
 */

#include "printk.h"
#include "string.h"
#include "tty.h"

void printk(const char *fmt, ...)
{
    char buf[1024];
    va_list args;
    va_start(args, fmt);
    vsprintf(buf, fmt, args);
    va_end(args);
    tty_write(buf);
}

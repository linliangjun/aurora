/**
 * 内核打印
 *
 * Copyright 2025 linliangjun
 */

#include "aurora/kernel/assert.h"
#include "aurora/kernel/tty.h"
#include "aurora/string.h"

void printk(const char *fmt, ...)
{
    char buf[1024];
    va_list args;
    va_start(args, fmt);
    size_t size = vsprintf(buf, fmt, args);
    ASSERT(size < sizeof(buf), "Buffer overflow!");
    va_end(args);
    tty_write(buf);
}

/**
 * STDIO
 *
 * Copyright 2026 linliangjun
 */

#include "stdio.h"
#include "aurora/uapi/sys_write.h"

void puts(const char *str)
{
    sys_write(str);
}

void sys_write(const char *str)
{
    __asm__ __volatile__(
        "movl $1, %%eax\n"
        "movl %0, %%ecx\n"          // str
        "int $0x80\n"
        :
        : "r"(str)
        : "%eax", "%ecx");
}

int printf(const char *format, ...)
{
    char buf[1024];
    va_list args;
    va_start(args, format);
    int size = vsprintf(buf, format, args);
    va_end(args);
    sys_write(buf);
    return size;
}

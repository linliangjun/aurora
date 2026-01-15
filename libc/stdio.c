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

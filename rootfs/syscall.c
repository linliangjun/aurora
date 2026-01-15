/**
 * 系统调用
 *
 * Copyright 2026 linliangjun
 */

#include "syscall.h"
#include "io.h"

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

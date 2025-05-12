/**
 * 内核主函数
 *
 * Copyright 2025 linliangjun
 */

#include "assert.h"

#define KERNEL_VERSION "0.0.1"

__attribute__((noreturn)) void main(void)
{
    PR_INFO("Kernel version \"%s\" (build %s %s)\n", KERNEL_VERSION, __DATE__, __TIME__);

    ASSERT(false, "End of instruction stream!");
    while (true)
        ;
}

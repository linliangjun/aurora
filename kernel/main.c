/**
 * 内核主函数
 *
 * Copyright 2025 linliangjun
 */

#include "kernel.h"
#include "printk.h"
#include "types.h"

__attribute__((noreturn)) void main(void)
{
    PR_INFO("Kernel version \"%s\" (build %s %s)\n", KERNEL_VERSION, __DATE__, __TIME__);

    while (true)
        ;
}

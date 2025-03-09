/**
 * 内核主函数
 *
 * Copyright 2025 linliangjun
 */

#include "aurora/kernel.h"
#include "aurora/printk.h"
#include "aurora/types.h"

__attribute__((noreturn)) void main(void)
{
    pr_info("Kernel name: %s, version: %s\n", KERNEL_NAME, KERNEL_VERSION);

    while (true)
        ;
}

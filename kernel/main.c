/**
 * 主函数
 *
 * Copyright 2024 linliangjun
 */

#include "aurora/kernel.h"
#include "aurora/printk.h"

void main(void)
{
    pr_info("kernel version: %s", KERNEL_VERSION);
}

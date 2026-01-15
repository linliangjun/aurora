/**
 * Panic
 *
 * Copyright 2025 linliangjun
 */

#ifndef __AURORA_KERNEL_PANIC_H
#define __AURORA_KERNEL_PANIC_H

#include "aurora/kernel/printk.h"
#include "aurora/types.h"

#define PANIC(fmt, ...)                   \
    printk("PANIC! " fmt, ##__VA_ARGS__); \
    while (true)                          \
        ;

#endif // __AURORA_KERNEL_PANIC_H

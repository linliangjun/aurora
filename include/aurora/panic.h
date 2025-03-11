/**
 * Panic
 *
 * Copyright 2025 linliangjun
 */

#ifndef __AURORA_PANIC_H
#define __AURORA_PANIC_H

#include "aurora/printk.h"
#include "stdlib.h"

#define panic(fmt, ...)                          \
    printk("KERNEL PANIC! " fmt, ##__VA_ARGS__); \
    abort()

#endif

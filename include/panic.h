/**
 * Panic
 *
 * Copyright 2025 linliangjun
 */

#ifndef __PANIC_H
#define __PANIC_H

#include "printk.h"
#include "types.h"

#define PANIC(fmt, ...)                   \
    printk("PANIC! " fmt, ##__VA_ARGS__); \
    while (true)                          \
        ;

#endif // __PANIC_H

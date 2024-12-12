/**
 * 断言
 *
 * Copyright 2024 linliangjun
 */

#include "assert.h"
#include "aurora/printk.h"

void __assert(const char *file, const int line, const char *exp)
{
    printk(KERN_EMERG "%s#%d - %s %s\n", file, line, "Assert failed.", exp);
    while (1)
        ;
}

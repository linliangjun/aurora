/**
 * Copyright 2025 linliangjun
 */

#include "assert.h"
#include "aurora/printk.h"
#include "stdlib.h"

void __bad_assertion(const char *exp, const char *file, unsigned int line)
{
    printk("Assertion \"%s\" failed, file \"%s\", line %d\n", exp, file, line);
    abort();
}

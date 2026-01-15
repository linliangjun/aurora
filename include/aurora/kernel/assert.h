/**
 * 断言
 *
 * Copyright 2025 linliangjun
 */

#ifndef __AURORA_KERNEL_ASSERT_H
#define __AURORA_KERNEL_ASSERT_H

#include "aurora/kernel/printk.h"
#include "aurora/types.h"

#ifdef NDEBUG

#define ASSERT(exp, msg)

#else

#define ASSERT(exp, msg)                                    \
    if (!(exp))                                             \
    {                                                       \
        PR_EMERG("Assertion '%s' failed. %s\n", #exp, msg); \
        while (true)                                        \
            ;                                               \
    }

#endif

#endif //   __AURORA_KERNEL_ASSERT_H

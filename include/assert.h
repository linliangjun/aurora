/**
 * 断言
 *
 * Copyright 2025 linliangjun
 */

#ifndef __ASSERT_H
#define __ASSERT_H

#include "printk.h"
#include "types.h"

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

#endif // __ASSERT_H

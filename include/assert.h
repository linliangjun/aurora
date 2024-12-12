/**
 * 断言
 *
 * Copyright 2024 linliangjun
 */

#ifndef __ASSERT_H
#define __ASSERT_H

#ifndef NDEBUG

#define assert(exp) if (!(exp)) __assert(__FILE__, __LINE__, #exp)

#else

#define assert(exp)

#endif

void __assert(const char *file, const int line, const char *exp);

#endif

/**
 * 断言
 *
 * Copyright 2025 linliangjun
 */

#ifndef __ASSERT_H
#define __ASSERT_H

#ifdef NDEBUG

#define assert(exp) ((void)0)

#else

void __bad_assertion(const char *exp, const char *file, unsigned int line);

#define assert(expr) ((expr) ? (void)0 : __bad_assertion(#expr, __FILE__, __LINE__))

#endif

#endif

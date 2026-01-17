/**
 * STDIO
 *
 * Copyright 2026 linliangjun
 */

#ifndef __LIBC_STDIO_H
#define __LIBC_STDIO_H

#include "stdarg.h"

#ifndef __SIZE_T
#define __SIZE_T

typedef unsigned int size_t;

#endif

void puts(const char *str);

int vsprintf(char *str, const char *format, va_list arg);

int printf(const char *format, ...);

#endif

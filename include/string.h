/**
 * 字符串工具库
 *
 * Copyright 2025 linliangjun
 */

#ifndef __STRING_H
#define __STRING_H

#include "vargs.h"

void *memcpy(void *dest, const void *src, size_t n);

void *memset(void *dest, u8 value, size_t n);

size_t strlen(const char *str);

size_t vsprintf(char *buf, const char *fmt, va_list args);

#endif // __STRING_H

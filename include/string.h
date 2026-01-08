/**
 * 字符串工具库
 *
 * Copyright 2025 linliangjun
 */

#ifndef __STRING_H
#define __STRING_H

#include "vargs.h"
#include "types.h"

void *memcpy(void *dest, const void *src, size_t n);

void *memset(void *dest, u8 value, size_t n);

size_t strlen(const char *str);

i32 strcmp(const char *s1, const char *s2);

i32 strncmp(const char *s1, const char *s2, size_t n);

char *strchr(const char *str, char ch);

char *strcat(char *dest, const char *src);

char *strcpy(char *dest, const char *src);

size_t vsprintf(char *buf, const char *fmt, va_list args);

#endif // __STRING_H

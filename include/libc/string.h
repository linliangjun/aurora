/**
 * STRING
 *
 * Copyright 2026 linliangjun
 */

#ifndef __LIBC_STRING_H
#define __LIBC_STRING_H

#ifndef __NULL
#define __NULL

#define NULL ((void *)0)

#endif

#ifndef __SIZE_T
#define __SIZE_T

typedef unsigned int size_t;

#endif

void *memcpy(void *dest, const void *src, size_t n);

void *memset(void *dest, int value, size_t n);

size_t strlen(const char *str);

int strcmp(const char *s1, const char *s2);

int strncmp(const char *s1, const char *s2, size_t n);

char *strchr(const char *str, int ch);

char *strcat(char *dest, const char *src);

char *strcpy(char *dest, const char *src);

#endif

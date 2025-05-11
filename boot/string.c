/**
 * 字符串工具库
 *
 * Copyright 2025 linliangjun
 */

#include "string.h"

void *memcpy(void *dest, const void *src, unsigned int n)
{
    char *pdest = (char *)dest, *psrc = (char *)src;
    while (n--)
        *pdest++ = *psrc++;
    return dest;
}

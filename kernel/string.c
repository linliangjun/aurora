/**
 * 字符串工具库
 *
 * Copyright 2025 linliangjun
 */

#include "string.h"

void *memcpy(void *dest, const void *src, size_t n)
{
    u8 *pdest = (u8 *)dest, *psrc = (u8 *)src;
    while (n--)
        *pdest++ = *psrc++;
    return dest;
}

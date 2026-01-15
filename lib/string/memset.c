/**
 * Copyright 2025 linliangjun
 */

#include "aurora/string.h"

void *memset(void *dest, u8 value, size_t n)
{
    u8 *pdest = (u8 *)dest;
    while (n--)
        *pdest++ = value;
    return dest;
}

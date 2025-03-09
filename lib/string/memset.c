/**
 * Copyright 2025 linliangjun
 */

#include "string.h"

void *memset(void *dest, char ch, unsigned int n)
{
    char *pdest = (char *)dest;
    while (n--)
        *pdest++ = ch;
    return dest;
}

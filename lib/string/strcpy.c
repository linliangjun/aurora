/**
 * Copyright 2025 linliangjun
 */

#include "string.h"

char *strcpy(char *dest, const char *src)
{
    char *ptr = dest;
    while (true)
    {
        *ptr++ = *src;
        if (*src++ == '\0')
            return dest;
    }
}

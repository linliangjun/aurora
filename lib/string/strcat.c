/**
 * Copyright 2025 linliangjun
 */

#include "string.h"

char *strcat(char *dest, const char *src)
{
    char *ptr = dest;
    while (*ptr != '\0')
        ptr++;
    while (true)
    {
        *ptr++ = *src;
        if (*src++ == '\0')
            return dest;
    }
}

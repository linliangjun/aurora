/**
 * Copyright 2026 linliangjun
 */

#include "string.h"

char *strchr(const char *str, char ch)
{
    char *ptr = (char *)str;
    while (true)
    {
        if (*ptr == ch)
            return ptr;
        if (*ptr++ == '\0')
            return (void *)0;
    }
}

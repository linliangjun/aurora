/**
 * Copyright 2025 linliangjun
 */

#include "string.h"

size_t strlen(const char *str)
{
    size_t len = 0;
    while (*str++)
        len++;
    return len;
}

/**
 * Copyright 2026 linliangjun
 */

#include "aurora/string.h"

i32 strncmp(const char *s1, const char *s2, size_t n)
{
    while (n > 0 && *s1 && (*s1 == *s2)) {
        s1++;
        s2++;
        n--;
    }
    if (n == 0)
        return 0;
    return *(u8 *)s1 - *(u8 *)s2;
}

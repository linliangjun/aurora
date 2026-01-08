/**
 * Copyright 2026 linliangjun
 */

#include "string.h"

i32 strcmp(const char *s1, const char *s2)
{
    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return *(u8 *)s1 < *(u8 *)s2 ? -1 : *(u8 *)s1 > *(u8 *)s2;
}

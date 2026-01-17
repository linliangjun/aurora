/**
 * STRING
 *
 * Copyright 2026 linliangjun
 */

#include "string.h"

#define EOS '\0'

void *memcpy(void *dest, const void *src, size_t n)
{
    char *pdest = (char *)dest, *psrc = (char *)src;
    while (n--)
        *pdest++ = *psrc++;
    return dest;
}

void *memset(void *dest, int value, size_t n)
{
    char *pdest = (char *)dest;
    while (n--)
        *pdest++ = value;
    return dest;
}

size_t strlen(const char *str)
{
    size_t len = 0;
    while (*str++)
        len++;
    return len;
}

int strcmp(const char *s1, const char *s2)
{
    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return *(unsigned char *)s1 < *(unsigned char *)s2 ? -1 : *(unsigned char *)s1 > *(unsigned char *)s2;
}

int strncmp(const char *s1, const char *s2, size_t n)
{
    while (n > 0 && *s1 && (*s1 == *s2)) {
        s1++;
        s2++;
        n--;
    }
    if (n == 0)
        return 0;
    return *(unsigned char *)s1 - *(unsigned char *)s2;
}

char *strchr(const char *str, int ch)
{
    char *ptr = (char *)str;
    while (1)
    {
        if (*ptr == ch)
            return ptr;
        if (*ptr++ == EOS)
            return NULL;
    }
}

char *strcat(char *dest, const char *src)
{
    char *ptr = dest;
    while (*ptr != EOS)
        ptr++;
    while (1)
    {
        *ptr++ = *src;
        if (*src++ == EOS)
            return dest;
    }
}

char *strcpy(char *dest, const char *src)
{
    char *ptr = dest;
    while (1)
    {
        *ptr++ = *src;
        if (*src++ == EOS)
            return dest;
    }
}

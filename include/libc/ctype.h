/**
 * CTYPE
 *
 * Copyright 2026 linliangjun
 */

#ifndef __CTYPE_H
#define __CTYPE_H

static inline int isdigit(char c)
{
    return c >= '0' && c <= '9';
}

static inline int toupper(int c)
{
    return c >= 'a' && c <= 'z' ? c - 32 : c;
}

#endif

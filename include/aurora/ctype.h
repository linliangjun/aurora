/**
 * 字符工具库
 *
 * Copyright 2025 linliangjun
 */

#ifndef __AURORA_CTYPE_H
#define __AURORA_CTYPE_H

#include "aurora/types.h"

static inline bool isdigit(char c)
{
    return c >= '0' && c <= '9';
}

static inline char toupper(char c)
{
    return c >= 'a' && c <= 'z' ? c - 32 : c;
}

#endif // __AURORA_CTYPE_H

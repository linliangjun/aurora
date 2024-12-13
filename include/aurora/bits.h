/**
 * 位操作
 *
 * Copyright 2024 linliangjun
 */

#ifndef __AURORA_BITS_H
#define __AURORA_BITS_H

#include "aurora/types.h"

static inline void setbit(u8 *num, u8 pos)
{
    *num |= (1 << pos);
}

static inline void clrbit(u8 *num, u8 pos)
{
    *num &= ~(1 << pos);
}

static inline bool getbit(u8 *num, u8 pos)
{
    return *num >> pos & 1;
}

#endif

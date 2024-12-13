/**
 * 位图
 *
 * Copyright 2024 linliangjun
 */

#ifndef __AURORA_BITMAP_H
#define __AURORA_BITMAP_H

#include "aurora/types.h"

typedef struct
{
    u32 total;
    u32 free;
    u32 allocated;
    u8 *data;
} bitmap_t;

void bitmap_init(bitmap_t *bitmap, u32 total);

void bitmap_set(bitmap_t *bitmap, u32 index);

void bitmap_clr(bitmap_t *bitmap, u32 index);

bool bitmap_get(bitmap_t *bitmap, u32 index);

#endif

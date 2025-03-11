/**
 * 位图
 *
 * Copyright 2025 linliangjun
 */

#ifndef __AURORA_BITMAP_H
#define __AURORA_BITMAP_H

#include "aurora/types.h"

/* 位图 */
typedef struct
{
    u32 total;     // 总数
    u32 free;      // 可用数量
    u32 allocated; // 已分配数量
    u8 *data;      // 数据
} bitmap_t;

void bitmap_init(bitmap_t *bitmap, u32 total);

bool bitmap_get(bitmap_t *bitmap, u32 index);

void bitmap_allocate(bitmap_t *bitmap, u32 index);

void bitmap_free(bitmap_t *bitmap, u32 index);

#endif

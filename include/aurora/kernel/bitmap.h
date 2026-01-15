/**
 * 位图
 *
 * Copyright 2025 linliangjun
 */

#ifndef __AURORA_KERNEL_BITMAP_H
#define __AURORA_KERNEL_BITMAP_H

#include "aurora/types.h"

/* 位图 */
typedef struct
{
    size_t total;  // 总数
    u32 free;      // 可用数量
    u32 allocated; // 已分配数量
    u8 *data;      // 数据
} bitmap_t;

void bitmap_init(bitmap_t *bitmap, size_t total);

bool bitmap_get(bitmap_t *bitmap, size_t index);

void bitmap_allocate(bitmap_t *bitmap, size_t index);

size_t bitmap_allocate_n(bitmap_t *bitmap, size_t n);

void bitmap_free(bitmap_t *bitmap, size_t index);

void bitmap_free_n(bitmap_t *bitmap, size_t index, size_t n);

#endif // __AURORA_KERNEL_BITMAP_H

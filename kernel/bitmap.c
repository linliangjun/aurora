/**
 * 位图
 *
 * Copyright 2024 linliangjun
 */

#include "aurora/bitmap.h"
#include "aurora/bits.h"
#include "string.h"
#include "assert.h"

#define DIV_ROUND_UP(n, d) ((n) + (d) - 1) / (d)

void bitmap_init(bitmap_t *bitmap, u32 total)
{
    bitmap->total = total;
    bitmap->free = total;
    bitmap->allocated = 0;
    memset(bitmap->data, 0, DIV_ROUND_UP(total, 8));
}

void bitmap_set(bitmap_t *bitmap, u32 index)
{
    assert(index < bitmap->total);
    u8 *data = bitmap->data + index / 8;
    u8 pos = index % 8;
    if (getbit(data, pos))
        return;
    setbit(data, pos);
    bitmap->free--;
    bitmap->allocated++;
}

void bitmap_clr(bitmap_t *bitmap, u32 index)
{
    assert(index < bitmap->total);
    u8 *data = bitmap->data + index / 8;
    u8 pos = index % 8;
    if (!getbit(data, pos))
        return;
    clrbit(data, pos);
    bitmap->free++;
    bitmap->allocated--;
}

bool bitmap_get(bitmap_t *bitmap, u32 index)
{
    assert(index < bitmap->total);
    return getbit(bitmap->data + index / 8, index % 8);
}

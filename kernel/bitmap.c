/**
 * 位图
 *
 * Copyright 2025 linliangjun
 */

#include "bitmap.h"
#include "bitops.h"
#include "assert.h"
#include "math.h"
#include "string.h"
#include "panic.h"

void bitmap_init(bitmap_t *bitmap, size_t total)
{
    bitmap->total = total;
    bitmap->free = total;
    bitmap->allocated = 0;
    memset(bitmap->data, 0, DIV_ROUND_UP(total, 8));
}

bool bitmap_get(bitmap_t *bitmap, size_t index)
{
    ASSERT(index < bitmap->total, "Index out of bound!");
    return getbit(*(bitmap->data + index / 8), index % 8);
}

void bitmap_allocate(bitmap_t *bitmap, size_t index)
{
    ASSERT(index < bitmap->total, "Index out of bound!");
    u8 *data = bitmap->data + index / 8;
    u8 pos = index % 8;
    if (getbit(*data, pos))
        return;
    setbit(data, pos);
    bitmap->free--;
    bitmap->allocated++;
}

size_t bitmap_allocate_n(bitmap_t *bitmap, size_t n)
{
    ASSERT(n > 0, "n must be greater than 0!");
    if (n > bitmap->free)
    {
        PANIC("Not enough free bits!");
    }
    size_t i = 0;
    while (i <= bitmap->total - n)
    {
        size_t j = n;
        bool found = true;
        while (j--)
        {
            if (bitmap_get(bitmap, i + j))
            {
                found = false;
                break;
            }
        }
        if (found)
        {
            for (size_t k = 0; k < n; k++)
                bitmap_allocate(bitmap, i + k);
            return i;
        }
        i += (j + 1);
    }
    PANIC("Not enough free bits!");
}

void bitmap_free(bitmap_t *bitmap, size_t index)
{
    ASSERT(index < bitmap->total, "Index out of bound!");
    u8 *data = bitmap->data + index / 8;
    u8 pos = index % 8;
    if (!getbit(*data, pos))
        return;
    clrbit(data, pos);
    bitmap->free++;
    bitmap->allocated--;
}

void bitmap_free_n(bitmap_t *bitmap, size_t index, size_t n)
{
    for (size_t i = 0; i < n; i++)
        bitmap_free(bitmap, index + i);
}

/**
 * 内存
 *
 * Copyright 2025 linliangjun
 */

#include "aurora/boot/mem.h"
#include "aurora/boot/bios_call.h"
#include "aurora/string.h"

typedef struct
{
    u32 base_low;
    u32 base_high;
    u32 len_low;
    u32 len_high;
    u32 type;
} ards_t;

u8 detect_mem_e820(mmap_t *mmap_vec, size_t *size)
{
    ards_t ards;
    u32 next = 0;
    size_t _size = 0;
    do
    {
        u8 code = int_15h_e820h(&next, (u32)&ards >> 4, (u32)&ards & 0xf);
        if (code)
            return code;
        mmap_t *mmap = mmap_vec + _size;
        mmap->base = ards.base_low;
        mmap->len = ards.len_low;
        mmap->type = ards.type;
        _size++;
    } while (next);
    *size = _size;
    return 0;
}

u8 detect_mem(mmap_t *mmap_vec, size_t *size)
{
    return detect_mem_e820(mmap_vec, size);
}

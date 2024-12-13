/**
 * 内存
 *
 * Copyright 2024 linliangjun
 */

#include "aurora/mem.h"
#include "aurora/bitmap.h"
#include "aurora/printk.h"
#include "assert.h"

#define PAGE_INDEX(addr) (addr) >> 12

extern ards_t *ards_vec;
extern u8 ards_vec_size;
extern void *__kernel_start;
extern void *__kernel_end;

static u32 mem_base;
static u32 mem_len;
static u32 mem_base_page;
static u8 bitmap_data[128 * 1024]; // 128 KiB 的位图
static bitmap_t bitmap = {.data = bitmap_data};

void mem_init(void)
{
    for (u8 i = 0; i < ards_vec_size; i++)
    {
        ards_t *ards = ards_vec + i;
        pr_debug("BIOS e820, base: %#.8x, len: %#.8x, type: %d\n",
                 ards->base_low, ards->len_low, ards->type);
        if (ards->type == ARDS_TYPE_AVL && ards->len_low > mem_len)
        {
            mem_base = ards->base_low;
            mem_len = ards->len_low;
        }
    }
    pr_info("Memory base: %#.8x, len: %#.8x\n", mem_base, mem_len);

    // 可用内存基地址必须 4 KiB 对齐
    assert(!(mem_base & 0xfff));
    mem_base_page = PAGE_INDEX(mem_base);

    // 设置位图
    bitmap_init(&bitmap, mem_len / 4096);
    for (u32 i = PAGE_INDEX((u32)&__kernel_start); i <= PAGE_INDEX((u32)&__kernel_end); i++)
    {
        if (i >= mem_base_page)
            bitmap_set(&bitmap, i - mem_base_page);
    }
    pr_info("Memory page, total: %d, free: %d, allocated: %d\n",
            bitmap.total, bitmap.free, bitmap.allocated);
}

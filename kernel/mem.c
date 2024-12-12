/**
 * 内存
 *
 * Copyright 2024 linliangjun
 */

#include "aurora/mem.h"
#include "aurora/printk.h"
#include "assert.h"

extern ards_t *ards_vec;
extern u8 ards_vec_size;

static u32 mem_base;
static u32 mem_len;

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
}

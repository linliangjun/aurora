/**
 * 物理内存管理器
 *
 * Copyright 2025 linliangjun
 */

#include "aurora/pmm.h"
#include "aurora/printk.h"
#include "string.h"

extern u8 __kernel_start[];
extern u8 __kernel_end[];

void pmm_init(const boot_params_t *boot_params)
{
    // 打印内存布局
    for (u32 i = 0; i < boot_params->mmap_entry_count; i++)
    {
        mmap_entry_t *mmap_entry = boot_params->mmap + i;
        pr_debug("Memory map - base = %#010x, len = %#10x, type = %d\n",
                 mmap_entry->base_low, mmap_entry->len_low, mmap_entry->type);
    }

    // 打印内核内存信息
    pr_info("Kernel in memory start = %#010x, end = %#010x, used = %d Byte\n",
            __kernel_start, __kernel_end, __kernel_end - __kernel_start);
}

/**
 * 物理内存管理器
 *
 * Copyright 2025 linliangjun
 */

#include "aurora/pmm.h"
#include "aurora/bitmap.h"
#include "aurora/panic.h"
#include "aurora/printk.h"
#include "string.h"

#define RANGE_CONTAINS(base, len, start, offset) \
    (start) < (base) ? false : (start) - (base) + (offset) <= (len)

#define PAGE_ADDR(index) (index) * PAGE_SIZE
#define PAGE_INDEX(addr) (addr) / PAGE_SIZE

#define PAGE_SIZE 4096                                // 页大小
#define PMM_MAX_SIZE (long long)4096 * 1024 * 1024    // 最大支持的内存大小（4 GiB）
#define PMM_MAX_PAGE_COUNT (PMM_MAX_SIZE) / PAGE_SIZE // 最大支持的页数量

extern u8 __kernel_start[];
extern u8 __kernel_end[];

// 物理内存位图
static u8 pmm_data[PMM_MAX_PAGE_COUNT / 8];
static bitmap_t pmm_bitmap = {.data = pmm_data};

static u32 kernel_page_start;
static u32 kernel_page_end;

static void setup_pmm_bitmap(const boot_params_t *boot_params)
{
    // 遍历物理页，检查是否可用
    for (u32 i = 0; i < pmm_bitmap.total; i++)
    {
        u32 page_addr = PAGE_ADDR(i);
        bool allocated = true;
        for (u32 j = 0; j < boot_params->mmap_entry_count; j++)
        {
            mmap_entry_t *mmap_entry = boot_params->mmap + j;
            if (!(RANGE_CONTAINS(mmap_entry->base_low, mmap_entry->len_low,
                                 page_addr, PAGE_SIZE)))
                continue;
            if (mmap_entry->type == MMAP_TYPE_UNUSED)
                allocated = false;
            break;
        }
        if (allocated)
            bitmap_allocate(&pmm_bitmap, i);
    }

    kernel_page_start = PAGE_INDEX((u32)__kernel_start);
    kernel_page_end = PAGE_INDEX((u32)__kernel_end) + 1;
    for (u32 i = kernel_page_start; i < kernel_page_end; i++)
        bitmap_allocate(&pmm_bitmap, i);
}

static void show_mem_info(const boot_params_t *boot_params)
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
    pr_info("Kernel in memory page start = %d, end = %d, count = %d\n",
            kernel_page_start, kernel_page_end, kernel_page_end - kernel_page_start);

    // 打印内存页信息
    pr_info("Phys memory page, total = %d, free = %d, allocated = %d\n",
            pmm_bitmap.total, pmm_bitmap.free, pmm_bitmap.allocated);
}

void pmm_init(const boot_params_t *boot_params)
{
    // 设置物理内存位图
    bitmap_init(&pmm_bitmap, PMM_MAX_PAGE_COUNT);
    setup_pmm_bitmap(boot_params);

    // 打印内存信息
    show_mem_info(boot_params);
}

u32 pmm_allocate_page(void)
{
    for (u32 i = 0; i < pmm_bitmap.total; i++)
    {
        if (bitmap_get(&pmm_bitmap, i))
            continue;
        bitmap_allocate(&pmm_bitmap, i);
        return i;
    }
    panic("Out of memory!\n");
}

void pmm_free_page(u32 page_index)
{
    bitmap_free(&pmm_bitmap, page_index);
}

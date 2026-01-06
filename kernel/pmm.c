/**
 * 物理内存
 *
 * Copyright 2025 linliangjun
 */

#include "pmm.h"
#include "bitmap.h"
#include "kernel.h"
#include "panic.h"

// 物理内存位图
static u8 pmm_data[DIV_ROUND_UP(PMM_MAX_PAGE_COUNT, 8)];
static bitmap_t pmm_bitmap = {.data = pmm_data};

void pmm_init(mmap_t *mmap, size_t mmap_size)
{
    for (size_t i = 0; i < mmap_size; i++)
    {
#ifdef DEBUG
        mmap_t *_mmap = mmap + i;
        PR_DEBUG("Memory base: %#010x, len: %#010x, type: %d\n",
                 _mmap->base, _mmap->len, _mmap->type);
#endif
    }

    // 设置物理内存位图
    bitmap_init(&pmm_bitmap, PMM_MAX_PAGE_COUNT);
    for (size_t i = 0; i < pmm_bitmap.total; i++)
    {
        bool allocated = true;
        for (size_t j = 0; j < mmap_size; j++)
        {
            mmap_t *_mmap = mmap + j;
            if (!RANGE_CONTAINS_PAGE(_mmap->base, _mmap->len, i))
                continue;
            if (_mmap->type == MMAP_TYPE_UNUSED)
            {
                allocated = false;
                break;
            }
        }
        if (allocated)
            bitmap_allocate(&pmm_bitmap, i);
    }

    // 将内核占用的物理页标记为已分配
    for (size_t i  = HHK_INIT_BSS_PAGE_START; i < HHK_INIT_BSS_PAGE_END; i++)
        bitmap_allocate(&pmm_bitmap, i);
    for (size_t i = KERNEL_PHYS_PAGE_START; i < KERNEL_PHYS_PAGE_END; i++)
        bitmap_allocate(&pmm_bitmap, i);

    PR_INFO("Memory page total: %u, free: %u, allocated: %u\n",
            pmm_bitmap.total, pmm_bitmap.free, pmm_bitmap.allocated);

    PR_DEBUG("Kernel phys addr: [%#010x, %#010x) size: %#uKiB\n",
        __kernel_phys_start, __kernel_phys_end, DIV_ROUND_UP(KERNEL_SIZE, 1024));
    PR_DEBUG("Kernel phys page: [%#u, %#u) count: %u\n",
        KERNEL_PHYS_PAGE_START, KERNEL_PHYS_PAGE_END, KERNEL_PAGE_COUNT);
}

size_t pmm_allocate_page(void)
{
    for (size_t i = 0; i < pmm_bitmap.total; i++)
    {
        if (bitmap_get(&pmm_bitmap, i))
            continue;
        bitmap_allocate(&pmm_bitmap, i);
        return i;
    }
    PANIC("Out of memory!\n");
}

void pmm_free_page(size_t page_index)
{
    bitmap_free(&pmm_bitmap, page_index);
}

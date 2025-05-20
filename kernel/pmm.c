/**
 * 物理内存
 *
 * Copyright 2025 linliangjun
 */

#include "pmm.h"
#include "bitmap.h"
#include "kernel.h"
#include "math.h"
#include "panic.h"

#define RANGE_CONTAINS(base, len, start, offset) \
    ((start) < (base) ? false : (start) - (base) + (offset) <= (len))
#define RANGE_CONTAINS_PAGE(base, len, page_index) \
    RANGE_CONTAINS(base, len, PAGE_ADDR(page_index), PAGE_SIZE)

#define PAGE_ADDR(index) ((index) * PAGE_SIZE)
#define PAGE_INDEX(addr) ((addr) / PAGE_SIZE)

extern u8 __kernel_start[];
extern u8 __kernel_end[];

// 物理内存位图
static u8 pmm_data[DIV_ROUND_UP(PMM_MAX_PAGE_COUNT, 8)];
static bitmap_t pmm_bitmap = {.data = pmm_data};

static size_t kernel_page_start;
static size_t kernel_page_end;

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
    kernel_page_start = PAGE_INDEX((uintptr_t)__kernel_start);
    kernel_page_end = PAGE_INDEX((uintptr_t)__kernel_end) + 1;
    for (size_t i = kernel_page_start; i < kernel_page_end; i++)
        bitmap_allocate(&pmm_bitmap, i);

    PR_INFO("Memory page total: %u, free: %u, allocated: %u\n",
            pmm_bitmap.total, pmm_bitmap.free, pmm_bitmap.allocated);

    PR_DEBUG("Kernel start: %#010x, end: %#010x, size: %#uKiB\n",
             __kernel_start, __kernel_end, DIV_ROUND_UP(__kernel_end - __kernel_start, 1024));
    PR_DEBUG("Kernel page start: %#u, end: %#u, count: %u\n", kernel_page_start,
             kernel_page_end, kernel_page_end - kernel_page_start);
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

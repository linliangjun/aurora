/**
 * 物理内存管理器
 *
 * Copyright 2025 linliangjun
 */

#ifndef __AURORA_KERNEL_PMM_H
#define __AURORA_KERNEL_PMM_H

#include "aurora/boot/mem.h"

/**
 * 初始化物理内存管理器
 */
void pmm_init(mmap_t *mmap, size_t mmap_size);

/**
 * 分配一页物理内存
 *
 * @return 物理内存页索引
 */
size_t pmm_allocate_page(void);

/**
 * 回收一页物理内存
 *
 * @param page_index 物理内存页索引
 */
void pmm_free_page(size_t page_index);

#endif // __AURORA_KERNEL_PMM_H

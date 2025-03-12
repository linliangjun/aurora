/**
 * 物理内存管理器
 *
 * Copyright 2025 linliangjun
 */

#ifndef __AURORA_PMM_H
#define __AURORA_PMM_H

#include "aurora/boot_params.h"
#include "aurora/mem.h"

void pmm_init(const boot_params_t *boot_params);

/**
 * 分配一页物理内存
 *
 * @return 物理内存页索引
 */
u32 pmm_allocate_page(void);

/**
 * 回收一页物理内存
 *
 * @param page_index 物理内存页索引
 */
void pmm_free_page(u32 page_index);

#endif

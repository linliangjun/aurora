/**
 * 内存
 *
 * Copyright 2025 linliangjun
 */

#ifndef __AURORA_MEM_H
#define __AURORA_MEM_H

#include "aurora/types.h"

extern u8 __kernel_start[];
extern u8 __kernel_end[];
extern u8 __kernel_phys_start[];

#define PAGE_SIZE 4096                       // 页大小
#define PAGE_INDEX(addr) (addr) / PAGE_SIZE  // 页索引
#define PAGE_ADDR(index) (index) * PAGE_SIZE // 页地址

#define PAGE_DIR_INDEX(addr) ((addr) >> 22) // 页目录索引

#define PHYS_ADDR(addr) ((addr) - (__kernel_start - __kernel_phys_start)) // 物理地址
#define PHYS_PAGE_INDEX(addr) (PAGE_INDEX(PHYS_ADDR(addr)))               // 物理页索引

#endif

/**
 * 内存
 *
 * Copyright 2025 linliangjun
 */

#ifndef __BOOT_MEM_H
#define __BOOT_MEM_H

#include "types.h"

#define MMAP_TYPE_UNUSED 1 // 未使用
#define MMAP_TYPE_RESV 2   // 保留

/* 内存映射信息 */
typedef struct
{
    u32 base;
    u32 len;
    u32 type;
} mmap_t;

/**
 * 获取内存映射信息
 *
 * @param mmap_vec 内存映射信息结构体数组
 * @param size MMAP 结构体数组大小
 */
u8 detect_mem(mmap_t *mmap_vec, size_t *size);

#endif // __BOOT_MEM_H

/**
 * 启动参数
 *
 * Copyright 2025 linliangjun
 */

#ifndef __AURORA_BOOT_PARAMS_H
#define __AURORA_BOOT_PARAMS_H

#include "aurora/types.h"

#define MMAP_TYPE_UNUSED 1 // 未使用
#define MMAP_TYPE_RESV 2   // 保留

typedef struct
{
    u32 base_low;
    u32 base_high;
    u32 len_low;
    u32 len_high;
    u32 type;
} mmap_entry_t;

typedef struct __attribute__((packed))
{
    u32 mmap_entry_count;
    mmap_entry_t *mmap;
} boot_params_t;

#endif

/**
 * 启动信息
 *
 * Copyright 2025 linliangjun
 */

#ifndef __BOOT_INFO_H
#define __BOOT_INFO_H

#include "boot/mem.h"

typedef struct
{
    u8 drive_id;
    mmap_t *mmap;
    size_t mmap_size;
} boot_info_t;

#endif // __BOOT_INFO_H

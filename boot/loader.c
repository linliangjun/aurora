/**
 * 内核加载器
 *
 * Copyright 2025 linliangjun
 */

#include "boot/loader.h"
#include "boot/boot.h"
#include "disk.h"

u8 load_kernel(u8 drive_id, uintptr_t *kernel_entry)
{
    *kernel_entry = KERNEL_ENTRY;
    return disk_read(drive_id, KERNEL_LBA, KERNEL_SECTOR_COUNT, (u8 *)KERNEL_ADDR);
}

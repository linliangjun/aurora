/**
 * 内核加载器
 *
 * Copyright 2025 linliangjun
 */

#include "kernel_loader.h"
#include "disk.h"
#include "config.h"

unsigned char load_kernel(unsigned char driver_id, unsigned int *kernel_entry)
{
    *kernel_entry = KERNEL_ENTRY;
    return disk_read(driver_id, KERNEL_LBA, KERNEL_SECTOR_COUNT, (char *)KERNEL_LMA);
}

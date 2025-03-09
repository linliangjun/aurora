/**
 * 内核加载器
 *
 * Copyright 2025 linliangjun
 */

#ifndef __BOOT_KERNEL_LOADER_H
#define __BOOT_KERNEL_LOADER_H

/**
 * 加载内核
 *
 * @param driver_id 驱动器号
 * @param kernel_entry 内核入口（调用成功后有效）
 *
 * @return 错误码，0 表示正常
 */
unsigned char load_kernel(unsigned char driver_id, unsigned int *kernel_entry);

#endif

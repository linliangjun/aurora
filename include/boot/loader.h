/**
 * 内核加载器
 *
 * Copyright 2025 linliangjun
 */

#ifndef __BOOT_LOADER_H
#define __BOOT_LOADER_H

#include "types.h"

/**
 * 加载内核
 *
 * @param drive_id 驱动器号
 * @param kernel_entry 内核入口（调用成功后有效）
 *
 * @return 错误码，0 表示正常
 */
u8 load_kernel(u8 drive_id, uintptr_t *kernel_entry);

#endif // __BOOT_LOADER_H

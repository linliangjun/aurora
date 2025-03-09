/**
 * 内核加载器
 *
 * Copyright 2025 linliangjun
 */

#ifndef __BOOT_KERNEL_LOADER_H
#define __BOOT_KERNEL_LOADER_H

#include "mem.h"

/**
 * 加载内核
 *
 * @param driver_id 驱动器号
 * @param kernel_entry 内核入口（调用成功后有效）
 * @param ards_vec ARDS 结构体数组
 * @param ards_vec_size ARDS 结构体数组大小
 *
 * @return 错误码，0 表示正常
 */
unsigned char load_kernel(unsigned char driver_id,
                          unsigned int *kernel_entry,
                          const ards_t *ards_vec,
                          unsigned int ards_vec_size);

#endif

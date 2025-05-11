/**
 * 磁盘驱动
 *
 * Copyright 2025 linliangjun
 */

#ifndef __DISK_H
#define __DISK_H

#include "types.h"

/**
 * 读取磁盘扇区
 *
 * @param drive_id 驱动器号
 * @param lba 逻辑扇区号
 * @param count 读取扇区数量
 * @param buf 缓冲区
 *
 * @return 错误码，0 表示正常
 */
u8 disk_read(u8 drive_id, u32 lba, u8 count, void *buf);

#endif // __DISK_H

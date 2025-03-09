/**
 * 磁盘驱动
 *
 * Copyright 2025 linliangjun
 */

#ifndef __BOOT_DISK_H
#define __BOOT_DISK_H

/**
 * 读取磁盘扇区
 *
 * @param driver_id 驱动器号
 * @param lba 逻辑扇区号
 * @param count 读取扇区数量
 * @param buf 缓冲区
 *
 * @return 错误码，0 表示正常
 */
unsigned char disk_read(unsigned char driver_id,
                        unsigned int lba,
                        unsigned char count,
                        char *buf);

#endif

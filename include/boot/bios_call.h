/**
 * BIOS 中断调用
 *
 * Copyright 2025 linliangjun
 */

#ifndef __BOOT_BIOS_CALL_H
#define __BOOT_BIOS_CALL_H

#define CYLINDER_SECTOR(c, s) (((c) << 8) | (((c) >> 2) & 0xc0) | (s))

#ifndef __ASSEMBLY__

#include "types.h"

/**
 * 以 TTY 模式打印字符
 *
 * @param c 字符
 */
void int_10h_0eh(char c);

/**
 * 以 CHS 模式读取磁盘扇区
 *
 * @param drive_id 驱动器号
 * @param cylinder_sector 柱面-扇区号
 * @param head 磁头号
 * @param count 读取扇区数量
 * @param seg 缓冲区段地址
 * @param offset 缓冲区偏移地址
 *
 * @return 错误码，0 表示正常
 */
u8 int_13h_02h(u8 drive_id, u16 cylinder_sector, u8 head, u8 count, u16 seg, u16 offset);

/**
 * 读取驱动器参数
 *
 * @param drive_id 驱动器号
 * @param max_cylinder_sector 最大柱面-扇区号（调用成功后有效）
 * @param max_head 最大磁头号（调用成功后有效）
 *
 * @return 错误码，0 表示正常
 */
u8 int_13h_08h(u8 drive_id, u16 *max_cylinder_sector, u8 *max_head);

/**
 * 获取内存布局
 *
 * @param next 后续值，首次调用置 0
 * @param seg ARDS 缓冲区段地址
 * @param offset ARDS 缓冲区偏移地址
 *
 * @return 错误码，0 表示正常
 */
u8 int_15h_e820h(u32 *next, u16 seg, u16 offset);

#endif

#endif // __BOOT_BIOS_CALL_H

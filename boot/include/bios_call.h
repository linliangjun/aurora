/**
 * BIOS 中断调用
 *
 * Copyright 2025 linliangjun
 */

#ifndef __BOOT_BIOS_CALL_H
#define __BOOT_BIOS_CALL_H

/**
 * 以 TTY 模式打印字符
 *
 * @param c 字符
 */
void int_10h_0eh(char c);

/**
 * 以 CHS 模式读取磁盘扇区
 *
 * @param driver_id 驱动器号
 * @param cylinder_sector 柱面-扇区号
 * @param head 磁头号
 * @param count 读取扇区数量
 * @param seg 缓冲区段地址
 * @param offset 缓冲区偏移地址
 *
 * @return 错误码，0 表示正常
 */
unsigned char int_13h_02h(unsigned char driver_id,
                          unsigned short cylinder_sector,
                          unsigned char head,
                          unsigned char count,
                          unsigned short seg,
                          unsigned short offset);

/**
 * 读取驱动器参数
 *
 * @param driver_id 驱动器号
 * @param max_cylinder_sector 最大柱面-扇区号（调用成功后有效）
 * @param max_head 最大磁头号（调用成功后有效）
 *
 * @return 错误码，0 表示正常
 */
unsigned char int_13h_08h(unsigned char driver_id,
                          unsigned short *max_cylinder_sector,
                          unsigned char *max_head);

/**
 * 获取内存布局
 *
 * @param next 后续值，首次调用置 0
 * @param seg ARDS 缓冲区段地址
 * @param offset ARDS 缓冲区偏移地址
 *
 * @return 错误码，0 表示正常
 */
unsigned char int_15h_e820h(unsigned int *next,
                            unsigned short seg,
                            unsigned short offset);

#endif

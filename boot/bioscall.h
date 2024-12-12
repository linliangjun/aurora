/**
 * BIOS 中断调用
 *
 * Copyright 2024 linliangjun
 */

#ifndef __BIOSCALL_H
#define __BIOSCALL_H

#define INT13H_CYLINDER_SECTOR(cylinder, sector) \
    ((cylinder) << 8) | (((cylinder) >> 2) & 0xc0) | (sector)

typedef struct
{
    unsigned short max_cylinder_sector;
    unsigned char max_head;
} driver_params_t;

typedef struct
{
    unsigned short cylinder_sector;
    unsigned char head;
    unsigned char count;
    unsigned short segment;
    unsigned short offset;
} driver_read_req_t;

typedef struct
{
    unsigned int base_low;
    unsigned int base_high;
    unsigned int len_low;
    unsigned int len_high;
    unsigned int type;
} ards_t;

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
 * @param driver_read_req_t 驱动器读取请求
 *
 * @return 错误码，0 表示正常
 */
unsigned char int_13h_02h(unsigned char driver_id, const driver_read_req_t *req);

/**
 * 读取驱动器参数
 *
 * @param driver_id 驱动器号
 * @param params 驱动器参数
 *
 * @return 错误码，0 表示正常
 */
unsigned char int_13h_08h(unsigned char driver_id, driver_params_t *params);

/**
 * 获取内存映射信息
 *
 * @param ards ards 结构体
 * @param next 后续值，首次调用置 0
 *
 * @return 错误码，0 表示正常
 */
unsigned char int_15h_e820h(ards_t *ards, unsigned int *next);

#endif

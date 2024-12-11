/**
 * 磁盘驱动
 *
 * Copyright 2024 linliangjun
 */

#ifndef __DISK_H
#define __DISK_H

typedef struct
{
    unsigned int lba;        // 逻辑扇区号
    unsigned char count;     // 读取扇区数量
    unsigned short segement; // 段地址
    unsigned short offset;   // 偏移地址
} disk_read_req_t;

unsigned char disk_read(unsigned char driver_id, const disk_read_req_t *req);

#endif

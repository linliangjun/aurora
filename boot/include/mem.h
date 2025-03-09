/**
 * 内存
 *
 * Copyright 2025 linliangjun
 */

#ifndef __BOOT_MEM_H
#define __BOOT_MEM_H

#define ARDS_TYPE_UNUSED 1 // 未使用
#define ARDS_TYPE_RESV 2   // 保留

typedef struct
{
    unsigned int base_low;
    unsigned int base_high;
    unsigned int len_low;
    unsigned int len_high;
    unsigned int type;
} ards_t;

/**
 * 获取内存布局
 *
 * @param ards_vec ARDS 结构体数组
 * @param size ARDS 结构体数组大小
 */
unsigned char detect_mem(ards_t *ards_vec, unsigned int *size);

#endif

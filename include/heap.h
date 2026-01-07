/**
 * 堆
 *
 * Copyright 2026 linliangjun
 */

#ifndef __HEAP_H
#define __HEAP_H

#include "types.h"

/**
 * 初始化堆
 */
void heap_init(void);

/**
 * 分配内核堆
 *
 * @param size 堆大小（字节）
 * @return 分配的堆指针
 */
void *heap_kmalloc(size_t size);

/**
 * 释放内核堆
 *
 * @param ptr 堆指针
 */
void heap_kfree(void *ptr);

#endif

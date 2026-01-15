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
 * 分配堆
 *
 * @param size 堆大小（字节）
 * @param user 是否为用户态堆，true：用户态；false：内核态
 * @return 分配的堆指针
 */
void *heap_malloc(size_t size, bool user);

/**
 * 释放堆
 *
 * @param ptr 堆指针
 */
void heap_free(void *ptr);

#endif

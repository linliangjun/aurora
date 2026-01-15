/**
 * 全局描述符表
 *
 * Copyright 2025 linliangjun
 */

#ifndef __AURORA_KERNEL_GDT_H
#define __AURORA_KERNEL_GDT_H

#include "aurora/seg.h"

/**
 * 初始化 GDT
 */
void gdt_init(void);

/**
 * 设置全局段描述符
 */
void gdt_set_desc(u16 index, const seg_desc_t *desc);

/**
 * 设置全局段描述符
 *
 * @return 段描述符索引
 */
u16 gdt_set_desc_auto(const seg_desc_t *desc);

#endif // __AURORA_KERNEL_GDT_H

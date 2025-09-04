/**
 * 全局描述符表
 *
 * Copyright 2025 linliangjun
 */

#ifndef __GDT_H
#define __GDT_H

#include "seg.h"

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

#endif // __GDT_H

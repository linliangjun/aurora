/**
 * 全局描述符表
 *
 * Copyright 2024 linliangjun
 */

#ifndef __AURORA_GDT_H
#define __AURORA_GDT_H

#include "aurora/seg.h"
#include "aurora/types.h"

void gdt_init(void);

void set_gdt_desc(u16 index, const seg_desc_t *desc);

#endif

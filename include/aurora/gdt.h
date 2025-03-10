/**
 * 全局描述符表
 *
 * Copyright 2025 linliangjun
 */

#ifndef __AURORA_GDT_H
#define __AURORA_GDT_H

#include "aurora/seg.h"

#define GDT_SIZE 128

#ifdef __ASSEMBLY__

#define CODE_SEG_SELE SEG_DESC_SELE(1, 0, 0)
#define DATA_SEG_SELE SEG_DESC_SELE(2, 0, 0)

#else

typedef struct __attribute__((__packed__))
{
    u16 lim;
    u32 base;
} gdt_desc_t;

void gdt_init(void);

void set_gdt_desc(u16 index, const seg_desc_t *desc);

#define CODE_SEG_SELE (seg_desc_sele_t){0, 0, 1}
#define DATA_SEG_SELE (seg_desc_sele_t){0, 0, 2}

#endif

#endif

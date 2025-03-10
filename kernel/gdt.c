/**
 * 全局描述符表
 *
 * Copyright 2025 linliangjun
 */

#include "aurora/gdt.h"
#include "aurora/printk.h"
#include "string.h"

static seg_desc_t gdt[GDT_SIZE] __attribute__((__aligned__(8)));

void gdt_init(void)
{
    set_gdt_desc(0, &SEG_DESC_NULL);
    set_gdt_desc(CODE_SEG_SELE.index, &SEG_DESC(0, 0xfffff, SEG_DESC_ACCESS(1, 0, 1, CODE_SEG), SEG_DESC_FLAGS(1, 1, 0)));
    set_gdt_desc(DATA_SEG_SELE.index, &SEG_DESC(0, 0xfffff, SEG_DESC_ACCESS(1, 0, 1, WRITEABLE_DATA_SEG), SEG_DESC_FLAGS(1, 1, 0)));
    gdt_desc_t gdt_desc = {.lim = sizeof(gdt) - 1, .base = (u32)gdt};
    __asm__ __volatile__("lgdt %0" : : "m"(gdt_desc));
    pr_info("Load GDT, base=%#x, limit=%#x\n", gdt_desc.base, gdt_desc.lim);
}

void set_gdt_desc(u16 index, const seg_desc_t *desc)
{
    memcpy(&gdt[index], desc, sizeof(seg_desc_t));
    pr_debug("Set GDT desc, index = %d, base = %#010x, lim = %#07x, g = %d, dpl = %d, "
             "s = %d, type = %04b, p = %d, db = %d, l = %d\n",
             index,
             (desc->base_high << 24) | desc->base_low,
             (desc->lim_high << 16) | desc->lim_low,
             desc->g, desc->dpl, desc->s, desc->type, desc->p, desc->db, desc->l);
}

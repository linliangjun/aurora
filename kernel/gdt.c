/**
 * 全局描述符表
 *
 * Copyright 2025 linliangjun
 */

#include "gdt.h"
#include "assert.h"
#include "kernel.h"
#include "string.h"

/**
 * GDT 描述符
 */
typedef struct __attribute__((packed))
{
    u16 lim;
    uintptr_t base;
} gdt_desc_t;

static seg_desc_t gdt[GDT_ENTRY_COUNT] __attribute__((aligned(8)));

void gdt_init(void)
{
    gdt_set_desc(0, &SEG_DESC_NULL);

    gdt_set_desc(CODE_SEG_SELE.index, &SEG_DESC(0, 0xfffff, SEG_DESC_ACCESS(1, 0, 1, SEG_DESC_TYPE_CODE), SEG_DESC_FLAGS(1, 1, 0)));
    gdt_set_desc(DATA_SEG_SELE.index, &SEG_DESC(0, 0xfffff, SEG_DESC_ACCESS(1, 0, 1, SEG_DESC_TYPE_WRITEABLE_DATA), SEG_DESC_FLAGS(1, 1, 0)));
    gdt_desc_t gdt_desc = {.lim = sizeof(gdt) - 1, .base = (uintptr_t)gdt};
    __asm__ __volatile__("lgdt %0" : : "m"(gdt_desc));
    PR_INFO("Load GDT, base: %#x, limit: %#x\n", gdt_desc.base, gdt_desc.lim);
}

void gdt_set_desc(u16 index, const seg_desc_t *desc)
{
    ASSERT(index < GDT_ENTRY_COUNT, "Index out of bound!");
    memcpy(gdt + index, desc, sizeof(seg_desc_t));
    PR_DEBUG("Set GDT[%d] base: %#010x, lim: %#07x, g: %d, dpl: %d, s: %d, type: %04b, p: %d, db: %d, l: %d\n",
             index,
             (desc->base_high << 24) | desc->base_low,
             (desc->lim_high << 16) | desc->lim_low,
             desc->g, desc->dpl, desc->s, desc->type, desc->p, desc->db, desc->l);
}

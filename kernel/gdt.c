/**
 * 全局描述符表
 *
 * Copyright 2024 linliangjun
 */

#include "aurora/gdt.h"
#include "aurora/kernel.h"
#include "aurora/printk.h"
#include "string.h"

static seg_desc_t gdt[GDT_SIZE] __attribute__((__aligned__(8)));

void gdt_init(void)
{
    set_gdt_desc(0, &SEG_DESC_NULL);
    set_gdt_desc(1, &SEG_DESC(0, 0xfffff, SEG_DESC_ACCESS(1, 0, 1, SEG_DESC_TYPE_CODE), SEG_DESC_FLAGS(1, 1, 0)));
    set_gdt_desc(2, &SEG_DESC(0, 0xfffff, SEG_DESC_ACCESS(1, 0, 1, SEG_DESC_TYPE_WRITEABLE_DATA), SEG_DESC_FLAGS(1, 1, 0)));
    typedef struct __attribute__((__packed__))
    {
        u16 lim;
        u32 base;
    } gdt_desc_t;
    gdt_desc_t gdt_desc = {.lim = sizeof(gdt) - 1, .base = (u32)gdt};
    __asm__ __volatile__("lgdt %0" : : "m"(gdt_desc));
    pr_debug("Load GDT, base=%#x, limit=%#x\n", gdt_desc.base, gdt_desc.lim);
}

void set_gdt_desc(u16 index, const seg_desc_t *desc)
{
    memcpy(&gdt[index], desc, sizeof(seg_desc_t));
}

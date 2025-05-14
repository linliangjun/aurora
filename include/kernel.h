/**
 * 内核头文件
 *
 * Copyright 2025 linliangjun
 */

#ifndef __KERNEL_H
#define __KERNEL_H

#include "seg.h"

#define KERNEL_VERSION "0.0.1"

#define KERNEL_STACK_SIZE (16 * 1024)

#define GDT_ENTRY_COUNT 128 // GDT 条目数量
#define IDT_ENTRY_COUNT 32  // IDT 条目数量

#ifdef __ASSEMBLY__

#define CODE_SEG_SELE SEG_DESC_SELE(1, 0, 0)
#define DATA_SEG_SELE SEG_DESC_SELE(2, 0, 0)

#else

#define CODE_SEG_SELE (seg_desc_sele_t){.rpl = 0, .ti = 0, .index = 1}
#define DATA_SEG_SELE (seg_desc_sele_t){.rpl = 0, .ti = 0, .index = 2}

#endif

#endif // __KERNEL_H

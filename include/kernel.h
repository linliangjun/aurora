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
#define IDT_ENTRY_COUNT 128 // IDT 条目数量

/* IRQ 起始向量号，必须是 8 的整倍数，即：低三位都是 0 */
#define PIC_MASTER_IRQ_VECTOR 32                         // 主片 IRQ 起始向量号
#define PIC_SLAVE_IRQ_VECTOR (PIC_MASTER_IRQ_VECTOR) + 8 // 从片 IRQ 起始向量号

#define JIFFY 10 // IRQ 0 产生间隔，单位：毫秒

#ifdef __ASSEMBLY__

#define CODE_SEG_SELE SEG_DESC_SELE(1, 0, 0)
#define DATA_SEG_SELE SEG_DESC_SELE(2, 0, 0)

#else

#define CODE_SEG_SELE (seg_desc_sele_t){.rpl = 0, .ti = 0, .index = 1}
#define DATA_SEG_SELE (seg_desc_sele_t){.rpl = 0, .ti = 0, .index = 2}

#endif

#define PAGE_SIZE 4096                                  // 页大小
#define PMM_MAX_SIZE ((u64)32 * 1024 * 1024)            // 最大支持的内存大小（32 MiB）
#define PMM_MAX_PAGE_COUNT ((PMM_MAX_SIZE) / PAGE_SIZE) // 最大支持的页数量

#endif // __KERNEL_H

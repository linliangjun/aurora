/**
 * 内核头文件
 *
 * Copyright 2025 linliangjun
 */

#ifndef __KERNEL_H
#define __KERNEL_H

#include "math.h"
#include "seg.h"

#define KERNEL_VERSION "0.0.1"

#define KERNEL_STACK_SIZE (16 * 1024)

#define GDT_ENTRY_COUNT 128 // GDT 条目数量
#define IDT_ENTRY_COUNT 128 // IDT 条目数量

/* IRQ 起始向量号，必须是 8 的整倍数，即：低三位都是 0 */
#define PIC_MASTER_IRQ_VECTOR 32                         // 主片 IRQ 起始向量号
#define PIC_SLAVE_IRQ_VECTOR (PIC_MASTER_IRQ_VECTOR) + 8 // 从片 IRQ 起始向量号

#define JIFFY 10 // IRQ 0 产生间隔，单位：毫秒

#define PAGE_SIZE 4096                         // 页大小
#define PAGE_ADDR(index) ((index) * PAGE_SIZE) // 页索引 -> 页起始地址
#define PAGE_INDEX(addr) ((addr) / PAGE_SIZE)  // 页起始地址 -> 页索引
#define RANGE_CONTAINS_PAGE(base, len, page_index) \
    RANGE_CONTAINS(base, len, PAGE_ADDR(page_index), PAGE_SIZE) // 范围是否包含某页

#define PMM_MAX_SIZE ((u64)32 * 1024 * 1024)            // 最大支持的物理内存大小（32 MiB）
#define PMM_MAX_PAGE_COUNT ((PMM_MAX_SIZE) / PAGE_SIZE) // 最大支持的物理页数量
#define KERNEL_PAGE_MAX_COUNT 1024                      // 内核最大页数量

#ifdef __ASSEMBLY__

#define CODE_SEG_SELE SEG_DESC_SELE(1, 0, 0)
#define DATA_SEG_SELE SEG_DESC_SELE(2, 0, 0)

#else

#define CODE_SEG_SELE (seg_desc_sele_t){.rpl = 0, .ti = 0, .index = 1}
#define DATA_SEG_SELE (seg_desc_sele_t){.rpl = 0, .ti = 0, .index = 2}

/* 内核起始和结束位置 */
extern u8 __hhk_init_bss_start[]; // hhk_init_bss 起始地址
extern u8 __hhk_init_bss_end[];   // hhk_init_bss 结束地址（不包含）

extern u8 __kernel_start[];      // 内核虚拟起始地址
extern u8 __kernel_end[];        // 内核虚拟结束地址（不包含）
extern u8 __kernel_phys_start[]; // 内核物理起始地址
extern u8 __kernel_phys_end[];   // 内核物理结束地址（不包含）

#define HHK_INIT_BSS_SIZE ((size_t)(__hhk_init_bss_end - __hhk_init_bss_start))   // hhk_init_bss 实际大小
#define HHK_INIT_BSS_PAGE_COUNT (DIV_ROUND_UP(HHK_INIT_BSS_SIZE, PAGE_SIZE))      // hhk_init_bss 物理页数量
#define HHK_INIT_BSS_PAGE_START PAGE_INDEX((uintptr_t)__hhk_init_bss_start)       // hhk_init_bss 物理起始页索引
#define HHK_INIT_BSS_PAGE_END (HHK_INIT_BSS_PAGE_START + HHK_INIT_BSS_PAGE_COUNT) // hhk_init_bss 物理结束页索引（不包含）

#define KERNEL_SIZE ((size_t)(__kernel_end - __kernel_start))             // 内核实际大小
#define KERNEL_PAGE_COUNT (DIV_ROUND_UP(KERNEL_SIZE, PAGE_SIZE))          // 内核实际页数量
#define KERNEL_PHYS_PAGE_START PAGE_INDEX((uintptr_t)__kernel_phys_start) // 内核物理起始页索引
#define KERNEL_PHYS_PAGE_END (KERNEL_PHYS_PAGE_START + KERNEL_PAGE_COUNT) // 内核物理结束页索引（不包含）
#define KERNEL_PAGE_START PAGE_INDEX((uintptr_t)__kernel_start)           // 内核虚拟起始页索引

#define KERNEL_HEAP_PAGE_COUNT 512 // 内核堆页数量

#define KEYBOARD_BUFFER_SIZE 256 // 键盘缓冲区大小

#define SHELL_CMD_BUFFER_SIZE 256 // Shell 命令缓冲区大小
#define SHELL_CWD_BUFFER_SIZE 256 // Shell 当前工作目录缓冲区大小
#define SHELL_CAT_BUFFER_SIZE 256 // Shell cat 命令缓冲区大小

extern u8 _binary_build_ramfs_tar_start[];
extern u8 _binary_build_ramfs_tar_end[];

#define RAMFS_START ((uintptr_t)_binary_build_ramfs_tar_start)
#define RAMFS_END ((uintptr_t)_binary_build_ramfs_tar_end)
#define RAMFS_SIZE ((size_t)(RAMFS_END - RAMFS_START))

#endif

#endif // __KERNEL_H

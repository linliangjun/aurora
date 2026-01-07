/**
 * 虚拟内存管理器
 *
 * Copyright 2025 linliangjun
 */

#ifndef __VMM_H
#define __VMM_H

#include "types.h"

/**
 * 页表项（PTE）
 */
typedef struct __attribute__((packed))
{
    u32 present : 1;   // P：0=页不存在，1=页存在
    u32 write : 1;     // R/W：0=只读，1=可写
    u32 user : 1;      // U/S：0=仅内核态可访问，1=用户态可访问
    u32 pwt : 1;       // PWT：0=Write-Back，1=Write-Through
    u32 pcd : 1;       // PCD：0=启用缓存，1=禁用缓存
    u32 accessed : 1;  // A：0=未访问，1=已访问（CPU 自动设置）
    u32 dirty : 1;     // D：0=未修改，1=已修改（CPU 自动设置，仅 PTE 有效）
    u32 pat : 1;       // PAT：页属性表索引（通常设为 0）
    u32 global : 1;    // G：0=普通页，1=全局页（切换 CR3 时不刷新 TLB）
    u32 available : 3; // 可用位（OS 可自由使用）
    u32 frame : 20;    // 物理页帧号（物理地址 >> 12）
} pte_t;

/**
 * 页目录项（PDE）
 */
typedef struct __attribute__((packed))
{
    u32 present : 1;   // P：0=页表不存在，1=页表存在
    u32 write : 1;     // R/W：0=只读，1=可写
    u32 user : 1;      // U/S：0=仅内核态可访问，1=用户态可访问
    u32 pwt : 1;       // PWT：0=Write-Back，1=Write-Through
    u32 pcd : 1;       // PCD：0=启用缓存，1=禁用缓存
    u32 accessed : 1;  // A：0=未访问，1=已访问（CPU 自动设置）
    u32 reserved : 1;  // 保留位（必须为 0）
    u32 ps : 1;        // PS：0=4KB 页，1=4MB 页（我们使用 4KB）
    u32 global : 1;    // G：全局页（仅当 PS=1 时有效）
    u32 available : 3; // 可用位（OS 可自由使用）
    u32 frame : 20;    // 物理页帧号（物理地址 >> 12）
} pde_t;

#define PDE_SIZE 1024      // 页目录项数量
#define PAGES_PER_PDE 1024 // 每个页目录项可以映射 1024 个页

/**
 * 初始化虚拟内存管理器
 */
void vmm_init(void);

/**
 * 分配一页内核虚拟内存
 *
 * @return 内核虚拟内存页索引
 */
size_t vmm_allocate_kernel_page(void);

/**
 * 回收一页内核虚拟内存
 *
 * @param page_index 内核虚拟内存页索引
 */
void vmm_free_kernel_page(size_t page_index);

#endif // __VMM_H

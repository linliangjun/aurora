/**
 * 高半核初始化
 *
 * Copyright 2025 linliangjun
 */

#include "kernel.h"
#include "vmm.h"

// 页目录表
static pde_t pde[PDE_SIZE] __attribute__((aligned(PAGE_SIZE), section(".hhk_init.bss")));

// 0 ~ 4MiB 的页表（恒等映射）
static pte_t page_table_0_4m[PAGES_PER_PDE] __attribute__((aligned(PAGE_SIZE), section(".hhk_init.bss")));

// 内核页表（映射到虚拟地址 0xc0000000）
static pte_t page_table_kernel[KERNEL_PAGE_MAX_COUNT] __attribute__((aligned(PAGE_SIZE), section(".hhk_init.bss")));

__attribute__((section(".hhk_init.text"))) static void hhk_painc(char *msg)
{
    while (true)
        ;
}

/**
 * 启用分页
 */
__attribute__((section(".hhk_init.text"))) static void enable_paging(pde_t *page_dir)
{
    // 加载页目录到 CR3
    __asm__ volatile("mov %0, %%cr3" : : "r"(page_dir));

    // 启用分页（设置 CR0.PG 位）
    u32 cr0;
    __asm__ volatile("mov %%cr0, %0" : "=r"(cr0));
    cr0 |= 0x80000000;
    __asm__ volatile("mov %0, %%cr0" : : "r"(cr0));
}

/**
 * 设置页目录项
 */
__attribute__((section(".hhk_init.text"))) static void set_pde(pde_t *pde, uintptr_t page_table_addr, bool writable, bool user)
{
    pde->present = 1;
    pde->write = writable ? 1 : 0;
    pde->user = user ? 1 : 0;
    pde->frame = PAGE_INDEX(page_table_addr);
}

/**
 * 设置页表项
 */
__attribute__((section(".hhk_init.text"))) static void set_pte(pte_t *pte, size_t page_frame, bool writable, bool user)
{
    pte->present = 1;
    pte->write = writable ? 1 : 0;
    pte->user = user ? 1 : 0;
    pte->frame = page_frame;
}

__attribute__((section(".hhk_init.text"))) void hhk_init(void)
{
    if (KERNEL_PAGE_COUNT > KERNEL_PAGE_MAX_COUNT)
        hhk_painc("Kernel page count exceed max limit\n");

    // PDE[0] 指向 page_table_0_4m（恒等映射 0~4MB）
    set_pde(&pde[0], (uintptr_t)page_table_0_4m, true, false);
    for (size_t i = 0; i < PAGES_PER_PDE; i++)
        set_pte(&page_table_0_4m[i], i, true, false);

    // 从 PDE[768] 开始，映射内核所占的页表
    for (size_t i = 0; i < DIV_ROUND_UP(KERNEL_PAGE_COUNT, PAGES_PER_PDE); i++)
        set_pde(&pde[768 + i], (uintptr_t)(page_table_kernel + i * PAGES_PER_PDE), true, false);
    for (size_t i = 0; i < KERNEL_PAGE_COUNT; i++)
        set_pte(&page_table_kernel[i], KERNEL_PHYS_PAGE_START + i, true, false);

    // 启用分页
    enable_paging(pde);
}

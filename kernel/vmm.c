/**
 * 虚拟内存管理器
 *
 * Copyright 2025 linliangjun
 */

#include "aurora/vmm.h"

static page_entry_dir_t page_dir_tab[1024] __attribute__((__aligned__(PAGE_SIZE)));          // 页目录表
static page_entry_dir_t kernel_page_tab[1024 * 256] __attribute__((__aligned__(PAGE_SIZE))); // 内核页表
static page_entry_dir_t low_mem_page_tab[1024] __attribute__((__aligned__(PAGE_SIZE)));      // 低端内存页表

__attribute__((__section__(".text.init"))) static void setup_page_entry_dir(
    page_entry_dir_t *page_entry_dir, u32 index)
{
    page_entry_dir->p = true;
    page_entry_dir->rw = 1;
    page_entry_dir->us = 0;
    page_entry_dir->index = index;
}

__attribute__((__section__(".text.init"))) void vmm_init(void)
{
    // 计算出页目录和内核页表的物理地址
    page_entry_dir_t *_page_dir_tab = (page_entry_dir_t *)PHYS_ADDR((u32)page_dir_tab);
    page_entry_dir_t *_kernel_page_tab = (page_entry_dir_t *)PHYS_ADDR((u32)kernel_page_tab);

    // 设置内核页目录
    u32 kernel_page_dir_start = PAGE_DIR_INDEX((u32)__kernel_start);
    u32 kernel_page_dir_end = PAGE_DIR_INDEX((u32)__kernel_end) + 1;
    for (u32 i = kernel_page_dir_start, j = 0; i < kernel_page_dir_end; i++, j++)
        setup_page_entry_dir(_page_dir_tab + i, PAGE_INDEX((u32)(_kernel_page_tab + j * 1024)));

    // 设置内核页表
    u32 kernel_phys_page_start = PHYS_PAGE_INDEX((u32)&__kernel_start);
    u32 kernel_phys_page_end = PHYS_PAGE_INDEX((u32)&__kernel_end) + 1;
    for (u32 i = kernel_phys_page_start, j = 0; i < kernel_phys_page_end; i++, j++)
        setup_page_entry_dir(_kernel_page_tab + j, i);

    // 设置低端 4 MiB 内存的映射（按原样映射）
    page_entry_dir_t *_low_mem_page_tab = (page_entry_dir_t *)PHYS_ADDR((u32)low_mem_page_tab);
    setup_page_entry_dir(_page_dir_tab, PAGE_INDEX((u32)_low_mem_page_tab));
    for (u32 i = 0; i < sizeof(low_mem_page_tab); i++)
        setup_page_entry_dir(_low_mem_page_tab + i, i);

    // 开启分页
    __asm__ __volatile__(
        "mov %0, %%eax\n\t"
        "mov %%eax, %%cr3\n\t"
        "mov %%cr0, %%eax\n\t"
        "or $0x80000000, %%eax\n\t"
        "mov %%eax, %%cr0\n\t"
        : : "a"(_page_dir_tab));
}

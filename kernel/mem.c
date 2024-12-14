/**
 * 内存
 *
 * Copyright 2024 linliangjun
 */

#include "aurora/mem.h"
#include "aurora/bitmap.h"
#include "aurora/printk.h"
#include "assert.h"

#define PAGE_INDEX(addr) ((addr) >> 12)
#define PAGE_DIR_INDEX(addr) ((addr) >> 22)

#define PHYS_ADDR(addr) ((addr) - (u32) & __kernel_start + 0x100000)
#define PHYS_PAGE_INDEX(addr) (PAGE_INDEX(PHYS_ADDR(addr)))

extern ards_t *ards_vec;
extern u8 ards_vec_size;
extern void *__kernel_start;
extern void *__kernel_end;

static u32 phys_mem_base;
static u32 phys_mem_len;
static u32 phys_mem_base_page;
static u8 bitmap_data[128 * 1024]; // 128 KiB 的位图
static bitmap_t bitmap = {.data = bitmap_data};

static page_entry_t page_dir_tab[1024] __attribute__((__aligned__(4096)));          // 页目录表
static page_entry_t kernel_page_tab[1024 * 256] __attribute__((__aligned__(4096))); // 内核页表
static page_entry_t low_mem_page_tab[1024] __attribute__((__aligned__(4096)));      // 低端内存页表

__attribute__((__section__(".text.startup"))) static void setup_page_entry(page_entry_t *page_entry, u32 index)
{
    page_entry->p = true;
    page_entry->rw = 1;
    page_entry->us = 0;
    page_entry->index = index;
}

__attribute__((__section__(".text.startup"))) void enable_mem_page(void)
{
    // 计算出页目录和页表的物理地址
    page_entry_t *_page_dir_tab = (page_entry_t *)PHYS_ADDR((u32)page_dir_tab);
    page_entry_t *_kernel_page_tab = (page_entry_t *)PHYS_ADDR((u32)kernel_page_tab);

    // 设置内核页目录
    for (u32 i = PAGE_DIR_INDEX((u32)&__kernel_start), j = 0;
         i <= PAGE_DIR_INDEX((u32)&__kernel_end); i++, j++)
        setup_page_entry((_page_dir_tab + i), PAGE_INDEX((u32)(_kernel_page_tab + j * 1024)));

    // 设置内核页表
    for (u32 i = PHYS_PAGE_INDEX((u32)&__kernel_start), j = 0;
         i <= PHYS_PAGE_INDEX((u32)&__kernel_end); i++, j++)
        setup_page_entry(_kernel_page_tab + j, i);

    // 设置低端 1 MiB 内存的映射（按原样映射）
    page_entry_t *_low_mem_page_tab = (page_entry_t *)PHYS_ADDR((u32)low_mem_page_tab);
    setup_page_entry(_page_dir_tab, PAGE_INDEX((u32)_low_mem_page_tab));
    for (u32 i = 0; i < 256; i++)
        setup_page_entry(_low_mem_page_tab + i, i);

    // 开启分页
    __asm__ __volatile__(
        "mov %0, %%eax\n\t"
        "mov %%eax, %%cr3\n\t"
        "mov %%cr0, %%eax\n\t"
        "or $0x80000000, %%eax\n\t"
        "mov %%eax, %%cr0"
        : : "a"(_page_dir_tab));
}

void mem_init(void)
{
    for (u8 i = 0; i < ards_vec_size; i++)
    {
        ards_t *ards = ards_vec + i;
        pr_debug("BIOS e820, base: %#.8x, len: %#.8x, type: %d\n",
                 ards->base_low, ards->len_low, ards->type);
        if (ards->type == ARDS_TYPE_AVL && ards->len_low > phys_mem_len)
        {
            phys_mem_base = ards->base_low;
            phys_mem_len = ards->len_low;
        }
    }
    pr_info("Phys memory base: %#.8x, len: %#.8x\n", phys_mem_base, phys_mem_len);

    // 可用内存基地址必须 4 KiB 对齐
    assert(!(phys_mem_base & 0xfff));
    phys_mem_base_page = PAGE_INDEX(phys_mem_base);

    // 设置位图
    bitmap_init(&bitmap, phys_mem_len / 4096);
    for (u32 i = PHYS_PAGE_INDEX((u32)&__kernel_start); i <= PHYS_PAGE_INDEX((u32)&__kernel_end); i++)
    {
        if (i >= phys_mem_base_page)
            bitmap_set(&bitmap, i - phys_mem_base_page);
    }
    pr_info("Phys memory page, total: %d, free: %d, allocated: %d\n",
            bitmap.total, bitmap.free, bitmap.allocated);
}

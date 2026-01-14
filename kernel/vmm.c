/**
 * 虚拟内存管理器
 *
 * Copyright 2026 linliangjun
 */

#include "vmm.h"
#include "bitmap.h"
#include "kernel.h"
#include "assert.h"
#include "pmm.h"
#include "string.h"

#define PTE(pde_index, pte_index) (0xffc00000 + (pde_index) * PAGE_SIZE + (pte_index) * sizeof(pte_t)) // PTE 指针
#define PDE(pde_index) PTE(1023, pde_index) // PDE 指针

// 内核虚拟内存位图
static u8 vmm_kernel_data[DIV_ROUND_UP(KERNEL_PAGE_MAX_COUNT, 8)];
static bitmap_t vmm_kernel_bitmap = {.data = vmm_kernel_data};

void vmm_init(void)
{
    ASSERT(KERNEL_PAGE_MAX_COUNT >= KERNEL_PAGE_COUNT, "Kernel size too large");
    bitmap_init(&vmm_kernel_bitmap, KERNEL_PAGE_MAX_COUNT);
    for (size_t i = 0; i < KERNEL_PAGE_COUNT; i++)
        bitmap_allocate(&vmm_kernel_bitmap, i);
    PR_INFO("VMM initialized\n");
}

static void invlpg(uintptr_t virtual_addr)
{
    __asm__ __volatile__("invlpg (%0)" : : "r"(virtual_addr) : "memory");
}

static void bind_page(size_t virtual_page_index, size_t phys_page_index, bool user)
{
    u16 pde_index = virtual_page_index / PAGES_PER_PDE;
    pde_t *pde = (pde_t *)PDE(pde_index);
    if (!pde->present)
    {
        size_t pte_page_index = pmm_allocate_page();
        pde->frame = pte_page_index;
        pde->present = true;
        pde->user = user;
        pde->write = true;
        memset((void *)PTE(pde_index, 0), 0, PAGE_SIZE);
    }
    u16 pte_index = virtual_page_index % PAGES_PER_PDE;
    pte_t *pte = (pte_t *)PTE(pde_index, pte_index);
    pte->present = true;
    pte->write = true;
    pte->user = false;
    pte->frame = phys_page_index;
    invlpg(PAGE_ADDR(virtual_page_index));
}

size_t vmm_allocate_kernel_pages(size_t n)
{
    size_t virtual_page_index = bitmap_allocate_n(&vmm_kernel_bitmap, n) + KERNEL_PAGE_START;
    for (size_t i = 0; i < n; i++)
    {
        size_t phys_page_index = pmm_allocate_page();
        bind_page(virtual_page_index + i, phys_page_index, false);
    }
    return virtual_page_index;
}

static void unbind_page(size_t virtual_page_index)
{
    u16 pde_index = virtual_page_index / PAGES_PER_PDE;
    pde_t *pde = (pde_t *)PDE(pde_index);
    if (!pde->present)
        return;

    u16 pte_index = virtual_page_index % PAGES_PER_PDE;
    pte_t *pte = (pte_t *)PTE(pde_index, pte_index);
    if (!pte->present)
        return;

    // 释放物理页
    pmm_free_page(pte->frame);

    pte->present = false;
    pte->frame = 0;
    invlpg(PAGE_ADDR(virtual_page_index));
}

void vmm_free_kernel_pages(size_t page_index, size_t n)
{
    bitmap_free_n(&vmm_kernel_bitmap, page_index - KERNEL_PAGE_START, n);
    for (size_t i = 0; i < n; i++)
        unbind_page(page_index + i);
}

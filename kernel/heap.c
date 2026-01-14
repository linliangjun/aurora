/**
 * 堆
 *
 * Copyright 2026 linliangjun
 */

#include "heap.h"
#include "vmm.h"
#include "kernel.h"
#include "assert.h"

typedef struct block_header {
    size_t size;               // 这个块的大小（包含 header 本身）
    bool is_free;              // 是否空闲
    struct block_header *next; // 指向下一个块
} block_header_t;

static const void *kheap;

void heap_init(void)
{
    size_t page_index = vmm_allocate_kernel_pages(KERNEL_HEAP_PAGE_COUNT);
    kheap = (void *)PAGE_ADDR(page_index);
    block_header_t *header = (block_header_t *)kheap;
    header->is_free = true;
    header->size = KERNEL_HEAP_PAGE_COUNT * PAGE_SIZE;
    header->next = 0;
    PR_INFO("Heap initialized, addr: %#x, size: %u\n", kheap, header->size);
}

void *heap_kmalloc(size_t size)
{
    block_header_t *header = (block_header_t *)kheap;
    bool found = false;
    do
    {
        if (header->is_free && header->size - sizeof(block_header_t) >= size)
        {
            found = true;
            break;
        }
        if (header->next != 0)
            header = header->next;
        else
            break;
    } while (!found);
    ASSERT(found, "Kernel heap out of memory");

    size_t old_size = header->size;
    size_t new_size = sizeof(block_header_t) + size;

    if (old_size > new_size + sizeof(block_header_t))
    {
        block_header_t *next = (block_header_t *)((uintptr_t)header + new_size);
        next->is_free = true;
        next->size = old_size - new_size;
        next->next = header->next;
        header->next = next;
    }
    else
        new_size = old_size;

    header->is_free = false;
    header->size = new_size;
    return (void *)(header + 1);
}

void heap_kfree(void *ptr)
{
    ASSERT((uintptr_t)ptr >= (uintptr_t)kheap
        && (uintptr_t)ptr < (uintptr_t)kheap + KERNEL_HEAP_PAGE_COUNT * PAGE_SIZE,
        "Invalid kernel heap pointer");
    block_header_t *header = (block_header_t *)ptr - 1;
    header->is_free = true;

    block_header_t *next = header->next;
    while (next != 0 && next->is_free)
    {
        header->size += next->size;
        header->next = next->next;
        next = next->next;
    }
}

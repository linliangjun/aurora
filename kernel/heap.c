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

static const void *user_heap;

void heap_init(void)
{
    // 分配内核堆
    size_t page_index = vmm_allocate_pages(KERNEL_HEAP_PAGE_COUNT, false);
    kheap = (void *)PAGE_ADDR(page_index);
    block_header_t *header = (block_header_t *)kheap;
    header->is_free = true;
    header->size = KERNEL_HEAP_PAGE_COUNT * PAGE_SIZE;
    header->next = 0;

    PR_INFO("Kernel heap initialized at %#x, size %#uKiB\n", kheap, DIV_ROUND_UP(KERNEL_HEAP_PAGE_COUNT * PAGE_SIZE, 1024));

    // 分配用户堆
    page_index = vmm_allocate_pages(USER_HEAP_PAGE_COUNT, true);
    user_heap = (void *)PAGE_ADDR(page_index);
    header = (block_header_t *)user_heap;
    header->is_free = true;
    header->size = USER_HEAP_PAGE_COUNT * PAGE_SIZE;
    header->next = 0;

    PR_INFO("User heap initialized at %#x, size %#uKiB\n", user_heap, DIV_ROUND_UP(USER_HEAP_PAGE_COUNT * PAGE_SIZE, 1024));
    PR_INFO("Heap initialized\n");
}

void *heap_malloc(size_t size, bool user)
{
    const void *heap = user ? user_heap : kheap;
    block_header_t *header = (block_header_t *)heap;
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
    ASSERT(found, "Heap out of memory");

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

void heap_free(void *ptr)
{
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

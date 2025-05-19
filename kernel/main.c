/**
 * 内核主函数
 *
 * Copyright 2025 linliangjun
 */

#include "boot/boot_info.h"
#include "kernel.h"
#include "pic.h"
#include "pit.h"
#include "printk.h"

static void kernel_init(boot_info_t *boot_info)
{
#ifdef DEBUG
    for (size_t i = 0; i < boot_info->mmap_size; i++)
    {
        mmap_t *mmap = boot_info->mmap + i;
        PR_DEBUG("Memory base: %#010x, len: %#010x, type: %d\n", mmap->base, mmap->len, mmap->type);
    }
#endif
    idt_init();
    pic_init();
    pit_init();
}

__attribute__((noreturn)) void main(boot_info_t *boot_info)
{
    kernel_init(boot_info);
    PR_INFO("Kernel version \"%s\" (build %s %s)\n", KERNEL_VERSION, __DATE__, __TIME__);
    while (true)
        ;
}

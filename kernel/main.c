/**
 * 内核主函数
 *
 * Copyright 2025 linliangjun
 */

#include "boot/boot_info.h"
#include "kernel.h"
#include "pic.h"
#include "pit.h"
#include "pmm.h"
#include "printk.h"

static void kernel_init(boot_info_t *boot_info)
{
    pmm_init(boot_info->mmap, boot_info->mmap_size);
    idt_init();
    pic_init();
    pit_init();
}

static void test(void)
{
#ifdef TEST
    while (true)
    {
        size_t index = pmm_allocate_page();
        PR_DEBUG("Allocate a memory page, index: %u\n", index);
    }
#endif
}

__attribute__((noreturn)) void main(boot_info_t *boot_info)
{
    kernel_init(boot_info);
    PR_INFO("Kernel version \"%s\" (build %s %s)\n", KERNEL_VERSION, __DATE__, __TIME__);
    test();
    while (true)
        ;
}

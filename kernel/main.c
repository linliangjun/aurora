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
#include "vmm.h"
#include "heap.h"
#include "printk.h"
#include "task_manager.h"
#include "keyboard.h"
#include "shell.h"

static void kernel_init(boot_info_t *boot_info)
{
    pmm_init(boot_info->mmap, boot_info->mmap_size);
    vmm_init();
    heap_init();
    idt_init();
    pic_init();
    pit_init();
    task_manager_init();
    keyboard_init();
}

__attribute__((noreturn)) void main(boot_info_t *boot_info)
{
    kernel_init(boot_info);
    task_spawn((uintptr_t)shell_main);
    while (true)
        task_manager_schedule();
}

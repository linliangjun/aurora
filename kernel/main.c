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
#include "printk.h"
#include "task_manager.h"

static void kernel_init(boot_info_t *boot_info)
{
    pmm_init(boot_info->mmap, boot_info->mmap_size);
    vmm_init();
    idt_init();
    pic_init();
    pit_init();
    task_manager_init();
}

static void task_1()
{
    while (true)
    {
        PR_INFO("Task 1 is running...\n");
        task_manager_schedule();
    }
}

static void task_2()
{
    while (true)
    {
        PR_INFO("Task 2 is running...\n");
        task_manager_schedule();
    }
}

__attribute__((noreturn)) void main(boot_info_t *boot_info)
{
    kernel_init(boot_info);
    PR_INFO("Kernel version \"%s\" (build %s %s)\n", KERNEL_VERSION, __DATE__, __TIME__);
    PR_INFO("Create task1 success, task index: %d\n", task_spawn((uintptr_t)task_1));
    PR_INFO("Create task2 success, task index: %d\n", task_spawn((uintptr_t)task_2));
    while (true)
        task_manager_schedule();
}

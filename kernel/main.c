/**
 * 内核主函数
 *
 * Copyright 2025 linliangjun
 */

#include "aurora/boot/boot_info.h"
#include "aurora/kernel/kernel.h"
#include "aurora/kernel/pic.h"
#include "aurora/kernel/pit.h"
#include "aurora/kernel/pmm.h"
#include "aurora/kernel/vmm.h"
#include "aurora/kernel/heap.h"
#include "aurora/kernel/printk.h"
#include "aurora/kernel/task_manager.h"
#include "aurora/kernel/keyboard.h"
#include "aurora/kernel/shell.h"
#include "aurora/kernel/ramfs.h"
#include "aurora/kernel/syscall_init.h"
#include "aurora/kernel/elf_loader.h"

static void kernel_init(boot_info_t *boot_info)
{
    pmm_init(boot_info->mmap, boot_info->mmap_size);
    vmm_init();
    heap_init();
    task_manager_init();
    ramfs_init(RAMFS_START, RAMFS_SIZE);
    idt_init();
    syscall_init();
    pic_init();
    pit_init();
    keyboard_init();
}

static void create_user_task(void)
{
    uintptr_t entry_point;
    i32 code = load_elf("user_task", &entry_point);
    if (code)
    {
        printk("Load user task failed: %d\n", code);
        return;
    }
    task_spawn(entry_point, true);
}

__attribute__((noreturn)) void main(boot_info_t *boot_info)
{
    kernel_init(boot_info);
    create_user_task();
    task_spawn((uintptr_t)shell_main, false);
    while (true)
        ;
}

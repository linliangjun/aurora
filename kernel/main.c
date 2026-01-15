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
#include "ramfs.h"
#include "syscall.h"
#include "elf_loader.h"

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

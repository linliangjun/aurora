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
#include "string.h"

static void kernel_init(boot_info_t *boot_info)
{
    pmm_init(boot_info->mmap, boot_info->mmap_size);
    vmm_init();
    heap_init();
    task_manager_init();
    ramfs_init(RAMFS_START, RAMFS_SIZE);
    idt_init();
    pic_init();
    pit_init();
    keyboard_init();
}

static void create_user_task(void)
{
    u8 code[] = {0xeb, 0xfe}; // 无限循环的机器码
    void *entry_point = heap_malloc(sizeof(code), true);
    memcpy(entry_point, code, sizeof(code));
    task_spawn((uintptr_t)entry_point, true);
}

__attribute__((noreturn)) void main(boot_info_t *boot_info)
{
    kernel_init(boot_info);
    create_user_task();
    task_spawn((uintptr_t)shell_main, false);
    while (true)
        ;
}

/**
 * 引导代码主函数
 *
 * Copyright 2025 linliangjun
 */

#include "aurora/boot/boot_info.h"
#include "aurora/boot/loader.h"
#include "aurora/boot/mem.h"
#include "aurora/boot/tty.h"

static mmap_t mmap[20];
static boot_info_t boot_info = {.mmap = mmap};

__attribute__((noreturn)) void main(u8 drive_id)
{
    tty_write("The second stage main is OK!\r\n");
    boot_info.drive_id = drive_id;

    // 获取内存布局
    u8 code = detect_mem(mmap, &boot_info.mmap_size);
    if (code)
    {
        tty_write("Detect memory failed.\r\n");
        while (true)
            ;
    }
    tty_write("Detect memory success.\r\n");

    // 加载内核
    uintptr_t kernel_entry;
    code = load_kernel(drive_id, &kernel_entry, mmap, boot_info.mmap_size);
    if (!code)
    {
        tty_write("Load kernel success.\r\n");

        // 跳转到内核入口
        __asm__ __volatile__(
            "push $boot_info\n\t"
            "jmp *%0" : : "a"(kernel_entry));
    }

    tty_write("Load kernel failed!\r\n");
    while (true)
        ;
}

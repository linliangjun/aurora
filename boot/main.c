/**
 * 引导代码主函数
 *
 * Copyright 2025 linliangjun
 */

#include "boot/loader.h"
#include "boot/mem.h"
#include "tty.h"

__attribute__((noreturn)) void main(u8 drive_id)
{
    tty_write("The second stage main is OK!\r\n");

    // 获取内存布局
    mmap_t mmap_vec[20];
    size_t mmap_vec_size;
    u8 code = detect_mem(mmap_vec, &mmap_vec_size);
    if (code)
    {
        tty_write("Detect memory failed.\r\n");
        while (true)
            ;
    }
    tty_write("Detect memory success.\r\n");

    // 加载内核
    uintptr_t kernel_entry;
    code = load_kernel(drive_id, &kernel_entry, mmap_vec, mmap_vec_size);
    if (!code)
    {
        tty_write("Load kernel success.\r\n");

        // 跳转到内核入口
        __asm__ __volatile__(
            "jmp *%0" : : "a"(kernel_entry));
    }

    tty_write("Load kernel failed!\r\n");
    while (true)
        ;
}

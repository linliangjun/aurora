/**
 * 内核主函数
 *
 * Copyright 2025 linliangjun
 */

#include "idt.h"
#include "kernel.h"
#include "printk.h"

static void kernel_init(void) {
    idt_init();
}

__attribute__((noreturn)) void main(void)
{
    kernel_init();
    PR_INFO("Kernel version \"%s\" (build %s %s)\n", KERNEL_VERSION, __DATE__, __TIME__);
    __asm__ __volatile__("int $0x80");
    while (true)
        ;
}

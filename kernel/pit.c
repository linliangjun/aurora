/**
 * 可编程间隔定时器
 *
 * Copyright 2025 linliangjun
 */

#include "pit.h"
#include "io.h"
#include "kernel.h"
#include "pic.h"
#include "printk.h"

#define IRQ0 0
#define PIT_CNT0_PORT 0x40
#define PIT_CMD_PORT 0x43

#define IRQ0_VECTOR PIC_M_IRQ_VECTOR

#define PIT_HZ 1193182 /* 振荡器频率 */

__attribute__((interrupt)) static void irq0_handler(const interrupt_frame_t *frame)
{
    send_eoi(IRQ0);
}

void pit_init(void)
{
    u16 count = PIT_HZ / 1000 * JIFFY;
    outb(0b00110100, PIT_CMD_PORT); // 配置定时器 0，工作模式 2，二进制模式
    outb(count, PIT_CNT0_PORT);
    outb( count >> 8, PIT_CNT0_PORT);
    set_irq(IRQ0, &INTERRUPT_GATE_DESC(CODE_SEG_SELE, (uintptr_t)irq0_handler, GATE_TYPE_INTERRUPT_32, 0, true), false);
    PR_INFO("PIT Initialized\n");
}

/**
 * 可编程中断控制器 8259
 *
 * Copyright 2024 linliangjun
 */

#include "pic.h"
#include "io.h"
#include "kernel.h"
#include "printk.h"

#define PIC_MASTER_PORT_0 0x20
#define PIC_MASTER_PORT_1 0x21
#define PIC_SLAVE_PORT_0 0xa0
#define PIC_SLAVE_PORT_1 0xa1

#define PIC_EOI 0x20 // 中断结束

static inline void set_mask(bool master, u8 mask)
{
    outb(mask, master ? PIC_MASTER_PORT_1 : PIC_SLAVE_PORT_1);
}

static inline u8 get_mask(bool master)
{
    return inb(master ? PIC_MASTER_PORT_1 : PIC_SLAVE_PORT_1);
}

void pic_init(void)
{
    // ICW1 写入偶地址端口
    outb(0b00010001, PIC_MASTER_PORT_0); // ICW1: 边沿触发, 级联, 需要ICW4
    outb(0b00010001, PIC_SLAVE_PORT_0);  // ICW1: 边沿触发, 级联, 需要ICW4

    // ICW2 ~ ICW4 写入奇地址端口
    outb(PIC_MASTER_IRQ_VECTOR, PIC_MASTER_PORT_1); // ICW2: 起始中断向量号 0x20
    outb(PIC_SLAVE_IRQ_VECTOR, PIC_SLAVE_PORT_1);   // ICW2: 起始中断向量号 0x28

    outb(0b00000100, PIC_MASTER_PORT_1); // ICW3: IR2接从片
    outb(2, PIC_SLAVE_PORT_1);           // ICW3: 设置从片连接到主片的 IR2 引脚

    outb(0b00000001, PIC_MASTER_PORT_1); // ICW4: 8086 模式, 非自动 EOI
    outb(0b00000001, PIC_MASTER_PORT_1); // ICW4: 8086 模式, 非自动 EOI

    // 屏蔽所有中断请求
    set_mask(true, 0xff);
    set_mask(false, 0xff);

    // 开启外中断
    __asm__ __volatile__("sti");
    PR_INFO("PIC Initialized\n");
}

void set_irq(u8 irq, interrupt_gate_desc_t *desc, bool mask)
{
    set_interrupt_gate_desc(irq + PIC_MASTER_IRQ_VECTOR, desc);
    if (mask)
        set_irq_mask(irq);
    else
        clr_irq_mask(irq);
}

void set_irq_mask(u8 irq)
{
    if (irq < 8)
        set_mask(true, get_mask(true) | (1 << irq));
    else
        set_mask(false, get_mask(false) | (1 << (irq - 8)));
}

void clr_irq_mask(u8 irq)
{
    if (irq < 8)
        set_mask(true, get_mask(true) & ~(1 << irq));
    else
        set_mask(false, get_mask(false) & ~(1 << (irq - 8)));
}

void send_eoi(u8 irq)
{
    if (irq >= 8)
        outb(PIC_EOI, PIC_SLAVE_PORT_0);
    outb(PIC_EOI, PIC_MASTER_PORT_0);
}

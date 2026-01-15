/**
 * 可编程中断控制器
 *
 * Copyright 2025 linliangjun
 */

#ifndef __AURORA_KERNEL_PIC_H
#define __AURORA_KERNEL_PIC_H

#include "aurora/kernel/idt.h"

/**
 * 初始化可编程中断控制器
 */
void pic_init(void);

/**
 * 设置中断请求
 *
 * @param irq 中断请求号
 * @param desc 中断门描述符
 * @param mask 掩码
 */
void set_irq(u8 irq, interrupt_gate_desc_t *desc, bool mask);

/**
 * 设置中断请求掩码
 *
 * @param irq 中断请求号
 */
void set_irq_mask(u8 irq);

/**
 * 清除中断请求掩码
 *
 * @param irq 中断请求号
 */
void clr_irq_mask(u8 irq);

/**
 * 发送中断结束指令
 *
 * @param irq 中断请求号
 */
void send_eoi(u8 irq);

#endif // __AURORA_KERNEL_PIC_H

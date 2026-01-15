/**
 * 键盘驱动
 *
 * Copyright 2026 linliangjun
 */

#include "aurora/kernel/keyboard.h"
#include "aurora/io.h"
#include "aurora/kernel/pic.h"
#include "aurora/kernel/printk.h"
#include "aurora/kernel/kernel.h"

#define KEYBOARD_IRQ 1     // 键盘 IRQ 号
#define KEYBOARD_PORT 0x60 // 键盘数据端口

static u8 keyboard_buffer[KEYBOARD_BUFFER_SIZE]; // 键盘缓冲区

static bool is_empty = true; // 缓冲区是否为空
static size_t head; // 缓冲区头指针
static size_t tail; // 缓冲区尾指针

__attribute__((interrupt)) static void irq1_handler(const interrupt_frame_t *frame)
{
    u8 scancode = inb(KEYBOARD_PORT);
    if (head == tail && !is_empty)
        PR_WARN("Keyboard buffer overflow, discarding scancode %#x\n", scancode);
    else
    {
        is_empty = false;
        keyboard_buffer[tail++] = scancode;
        tail %= KEYBOARD_BUFFER_SIZE;
    }
    send_eoi(KEYBOARD_IRQ);
}

/**
 * 初始化键盘驱动
 */
void keyboard_init(void)
{
    set_irq(KEYBOARD_IRQ, &INTERRUPT_GATE_DESC(CODE_SEG_SELE, (uintptr_t)irq1_handler, GATE_TYPE_INTERRUPT_32, 0, true), false);
    PR_INFO("Keyboard Initialized\n");
}

i16 keyboard_get_scancode(void)
{
    if (is_empty)
        return -1;
    u8 scancode = keyboard_buffer[head++];
    head %= KEYBOARD_BUFFER_SIZE;
    is_empty = head == tail;
    return scancode;
}

/**
 * in / out 指令
 *
 * Copyright 2025 linliangjun
 */

#ifndef __AURORA_IO_H
#define __AURORA_IO_H

#include "aurora/types.h"

static inline void outb(u8 value, u16 port)
{
    __asm__ __volatile__("outb %b0, %w1" : : "a"(value), "Nd"(port));
}

static inline void outw(u16 value, u16 port)
{
    __asm__ __volatile__("outw %w0, %w1" : : "a"(value), "Nd"(port));
}

static inline u8 inb(u16 port)
{
    u8 value;
    __asm__ __volatile__("inb %w1, %b0" : "=a"(value) : "Nd"(port));
    return value;
}
static inline u16 inw(u16 port)
{
    u16 value;
    __asm__ __volatile__("inw %w1, %w0" : "=a"(value) : "Nd"(port));
    return value;
}

#endif // __AURORA_IO_H

/**
 * IO
 *
 * Copyright 2024 linliangjun
 */

#ifndef AURORA_IO_H
#define AURORA_IO_H

#include <stdint.h>

#define PORT_CRT_ADDR 0x3D4
#define PORT_CRT_DATA 0x3D5

static inline void outb(uint8_t value, uint16_t port)
{
    __asm__ __volatile__("outb %b0, %w1" : : "a"(value), "Nd"(port));
}

static inline void outw(uint16_t value, uint16_t port)
{
    __asm__ __volatile__("outw %w0, %w1" : : "a"(value), "Nd"(port));
}

static inline uint8_t inb(uint16_t port)
{
    uint8_t value;
    __asm__ __volatile__("inb %w1, %b0" : "=a"(value) : "Nd"(port));
    return value;
}
static inline uint16_t inw(uint16_t port)
{
    uint16_t value;
    __asm__ __volatile__("inw %w1, %w0" : "=a"(value) : "Nd"(port));
    return value;
}

#endif // AURORA_IO_H

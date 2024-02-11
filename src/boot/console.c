/**
 * 控制台驱动
 *
 * Copyright 2024 linliangjun
 */

#include "aurora/console.h"

#include "aurora/io.h"

#include <stddef.h>
#include <string.h>

#define CRT_CURSOR_HIGH 0xE // 光标位置 - 高位
#define CRT_CURSOR_LOW 0xF  // 光标位置 - 低位

int8_t *console_cursor_get(void)
{
    outb(CRT_CURSOR_HIGH, PORT_CRT_ADDR);
    uint16_t pos = inb(PORT_CRT_DATA) << 8;
    outb(CRT_CURSOR_LOW, PORT_CRT_ADDR);
    pos += inb(PORT_CRT_DATA);
    return (int8_t *)((pos << 1) + 0xb8000);
}

static void _set_cursor(const int8_t *ptr)
{
    uint16_t pos = ((uint32_t)ptr - 0xb8000) >> 1;
    outb(CRT_CURSOR_LOW, PORT_CRT_ADDR);
    outb(pos, PORT_CRT_DATA);
    outb(CRT_CURSOR_HIGH, PORT_CRT_ADDR);
    outb(pos >> 8, PORT_CRT_DATA);
}

static int8_t *_scroll_up(int8_t *ptr)
{
    if ((size_t)ptr < 0xb8fa0)
    {
        return ptr;
    }
    ptr -= 160;
    memcpy((int8_t *)0xb8000, (int8_t *)0xb80a0, 3840);
    int8_t *p = (int8_t *)0xb8f00;
    for (size_t i = 0; i < 80; i++)
    {
        *p++ = ' ';
        *p++ = 7;
    }
    return ptr;
}

static int8_t *_putc(int8_t *ptr, int8_t ch)
{
    switch (ch)
    {
    case '\n':
        ptr += (160 - ((size_t)ptr - 0xb8000) % 160);
        ptr = _scroll_up(ptr);
        break;
    default:
        ptr = _scroll_up(ptr);
        *ptr++ = ch;
        *ptr++ = 7;
    }
    return ptr;
}

int8_t *console_write(int8_t *ptr, const int8_t *str, bool new_line, bool update_cursor)
{
    if (new_line)
    {
        size_t count = (size_t)(ptr - 0xb8000) % 160;
        if (count != 0)
        {
            ptr += (160 - count);
        }
    }
    int8_t ch;
    while ((ch = *str++) != '\0')
    {
        ptr = _putc(ptr, ch);
    }
    if (update_cursor)
    {
        _set_cursor(ptr);
    }
    return ptr;
}

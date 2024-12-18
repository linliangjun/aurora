/**
 * 内核打印
 *
 * Copyright 2024 linliangjun
 */

#include "aurora/printk.h"
#include "aurora/console.h"
#include "vsprintf.h"
#include "assert.h"

void printk(const char *fmt, ...)
{
    char buf[1024];

    va_list args;
    va_start(args, fmt);
    u32 size = vsprintf(buf, fmt, args);
    assert(size <= 1024);
    va_end(args);

    char *video_text_ptr = get_cursor();
    video_text_ptr = console_write(video_text_ptr, buf);
    set_cursor(video_text_ptr);
}

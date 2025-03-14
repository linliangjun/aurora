/**
 * 内核打印
 *
 * Copyright 2025 linliangjun
 */

#include "aurora/printk.h"
#include "aurora/console.h"
#include "assert.h"
#include "stdio.h"

void printk(const char *fmt, ...)
{
    char buf[1024];

    va_list args;
    va_start(args, fmt);
    u32 size = vsprintf(buf, fmt, args);
    va_end(args);

    assert(size <= sizeof(buf));

    char *video_text_ptr = get_cursor();
    video_text_ptr = console_write(video_text_ptr, buf);
    set_cursor(video_text_ptr);
}

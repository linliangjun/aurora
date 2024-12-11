/**
 * 主函数
 *
 * Copyright 2024 linliangjun
 */

#include "aurora/console.h"

void main(void)
{
    char *video_text_ptr = get_cursor();
    video_text_ptr = console_write(video_text_ptr, "Kernel main OK!\n");
    set_cursor(video_text_ptr);
}

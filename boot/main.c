/**
 * 引导代码主函数
 *
 * Copyright 2025 linliangjun
 */

static const char msg[] = "The second stage main is OK!";

__attribute__((noreturn)) void main(unsigned char drive_id)
{
    short *frame_buffer = (short *)0xb8000;
    for (unsigned int i = 0; i < sizeof(msg) - 1; i++)
        frame_buffer[i] = (14 << 8) | msg[i];
    while (1)
        ;
}

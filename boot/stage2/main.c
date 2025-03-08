/**
 * 第二阶段引导代码主函数
 *
 * Copyright 2025 linliangjun
 */

__attribute__((noreturn)) void main(unsigned char driver_id)
{
    char msg[] = "The second stage main, OK!";
    short *vbuf = (short *)0xb8000;
    for (unsigned int i = 0; i < sizeof(msg) - 1; i++)
        vbuf[i] = (14 << 8) | msg[i];
    while (1)
        ;
}

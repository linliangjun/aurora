/**
 * 第二阶段引导代码主函数
 *
 * Copyright 2025 linliangjun
 */

#include "bios_call.h"

__attribute__((noreturn)) void main(unsigned char driver_id)
{
    char msg[] = "The second stage main, OK!";
    for (unsigned int i = 0; i < sizeof(msg) - 1; i++)
        int_10h_0eh(msg[i]);
    while (1)
        ;
}

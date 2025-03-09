/**
 * 第二阶段引导代码主函数
 *
 * Copyright 2025 linliangjun
 */

#include "console.h"

__attribute__((noreturn)) void main(unsigned char driver_id)
{
    console_write("The second stage main, OK!");
    while (1)
        ;
}

/**
 * 引导代码主函数
 *
 * Copyright 2025 linliangjun
 */

#include "tty.h"
#include "types.h"

__attribute__((noreturn)) void main(u8 drive_id)
{
    tty_write("The second stage main is OK!\r\n");
    while (true)
        ;
}

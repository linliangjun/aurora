/**
 * 第二阶段引导代码主函数
 *
 * Copyright 2025 linliangjun
 */

#include "console.h"
#include "kernel_loader.h"

#define half_byte_to_hex(n) (n) < 10 ? (n) + '0' : (n) - 10 + 'a'

void btox(unsigned char n, char *str)
{
    *str++ = half_byte_to_hex(n >> 8);
    *str = half_byte_to_hex(n & 0xf);
}

void assert_is_zero(unsigned char code, char *msg)
{
    if (!code)
        return;

    // 转换错误码
    char error_code[] = "  \r\n";
    btox(code, error_code);

    // 打印错误消息和错误码
    console_write(msg);
    console_write(" Error code: 0x");
    console_write(error_code);
    while (1)
        ;
}

__attribute__((noreturn)) void main(unsigned char driver_id)
{
    console_write("The second stage main, OK!\r\n");

    // 加载内核
    unsigned int kernel_entry;
    unsigned char code = load_kernel(driver_id, &kernel_entry);
    assert_is_zero(code, "Load kernel failed!");

    console_write("Load kernel success.\r\n");

    // 跳转到内核入口
    __asm__ __volatile__(
        "jmp *%0" : : "a"(kernel_entry));
    while (1)
        ;
}

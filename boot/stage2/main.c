/**
 * 第二阶段引导代码主函数
 *
 * Copyright 2025 linliangjun
 */

#include "console.h"
#include "kernel_loader.h"
#include "mem.h"

#define half_byte_to_hex(n) (n) < 10 ? (n) + '0' : (n) - 10 + 'a'

typedef struct __attribute__((packed))
{
    unsigned int ards_vec_size;
    ards_t *ards_vec;
} boot_params_t;

static boot_params_t boot_params;

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

    // 获取内存布局
    ards_t ards_vec[20];
    unsigned int ards_vec_size;
    unsigned char code = detect_mem(ards_vec, &ards_vec_size);
    assert_is_zero(code, "Detect mem failed.");

    console_write("Detect mem success.\r\n");

    boot_params.ards_vec_size = ards_vec_size;
    boot_params.ards_vec = ards_vec;

    // 加载内核
    unsigned int kernel_entry;
    code = load_kernel(driver_id, &kernel_entry, ards_vec, ards_vec_size);
    assert_is_zero(code, "Load kernel failed!");

    console_write("Load kernel success.\r\n");

    // 跳转到内核入口
    __asm__ __volatile__(
        "push $boot_params\n\t"
        "jmp *%0" : : "a"(kernel_entry));
    while (1)
        ;
}

/**
 * 主函数
 *
 * Copyright 2024 linliangjun
 */

#include "bioscall.h"
#include "loader.h"

#define MSG(str)                                       \
    for (unsigned int i = 0; i < sizeof(str) - 1; i++) \
    int_10h_0eh(str[i])

static const char notification_string[] = "Boot main OK!\r\n";
static const char detect_memory_error_string[] = "Detect memory failed.\r\n";
static const char load_kernel_error_string[] = "Load kernel failed.\r\n";

ards_t ards_vec[20];
unsigned char ards_vec_size;
unsigned int kernel_entry;

/**
 * 获取内存映射信息
 *
 * @return 错误码，0 表示正常
 */
unsigned char detect_memory(void)
{
    unsigned int next = 0;
    unsigned char code;
    do
    {
        code = int_15h_e820h(ards_vec + ards_vec_size, &next);
        ards_vec_size++;
    } while (!code && next);
    return code;
}

unsigned char main(unsigned char driver_id)
{
    MSG(notification_string);
    if(detect_memory())
    {
        MSG(detect_memory_error_string);
        return 1;
    }
    if (load_kernel(driver_id, &kernel_entry))
    {
        MSG(load_kernel_error_string);
        return 1;
    }
    return 0;
}

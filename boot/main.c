/**
 * 主函数
 *
 * Copyright 2024 linliangjun
 */

#include "bioscall.h"
#include "disk.h"
#include "string.h"

#define KERNEL_LBA 32
#define KERNEL_SECTOR_COUNT 200
#define KERNEL_ADDR 0x100000

#define MSG(str)                                       \
    for (unsigned int i = 0; i < sizeof(str) - 1; i++) \
    int_10h_0eh(str[i])

static const char notification_string[] = "Boot main OK!\r\n";
static const char detect_memory_error_string[] = "Detect memory failed.\r\n";
static const char load_kernel_error_string[] = "Load kernel failed.\r\n";

ards_t ards_vec[20];
unsigned char ards_vec_size;

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

/**
 * 加载内核
 *
 * @param driver_id 驱动器号
 *
 * @return 错误码，0 表示正常
 */
static unsigned char load_kernel(unsigned char driver_id)
{
    char buf[512];
    disk_read_req_t req = {.count = 1, .segement = 0, .offset = (unsigned int)buf};
    for (unsigned int i = 0; i < KERNEL_SECTOR_COUNT; i++)
    {
        req.lba = KERNEL_LBA + i;
        unsigned char code = disk_read(driver_id, &req);
        if (code)
            return code;
        memcpy((void *)(KERNEL_ADDR + i * 512), buf, 512);
    }
    return 0;
}

unsigned char main(unsigned char driver_id)
{
    MSG(notification_string);
    if(detect_memory())
    {
        MSG(detect_memory_error_string);
        return 1;
    }
    if (load_kernel(driver_id))
    {
        MSG(load_kernel_error_string);
        return 1;
    }
    return 0;
}

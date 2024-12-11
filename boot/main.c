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
static const char load_kernel_error_string[] = "Load kernel failed.\r\n";

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

void main(unsigned char driver_id)
{
    MSG(notification_string);
    if (load_kernel(driver_id))
        MSG(load_kernel_error_string);
    else
        __asm__ __volatile__("jmp *%0" : : "a"(KERNEL_ADDR));
}

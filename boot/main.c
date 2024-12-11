/**
 * 主函数
 *
 * Copyright 2024 linliangjun
 */

#include "bioscall.h"
#include "disk.h"

#define MSG(str)                                       \
    for (unsigned int i = 0; i < sizeof(str) - 1; i++) \
    int_10h_0eh(str[i])

static const char notification_string[] = "Boot main OK!\r\n";
static const char disk_read_error_string[] = "disk read error\r\n";

void main(unsigned char driver_id)
{
    MSG(notification_string);
    disk_read_req_t req = {0, 1, 0, 0x500};
    unsigned char code = disk_read(driver_id, &req);
    if (code)
        MSG(disk_read_error_string);
}

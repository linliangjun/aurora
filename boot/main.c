/**
 * 主函数
 *
 * Copyright 2024 linliangjun
 */

#include "bioscall.h"

#define MSG(str)                                       \
    for (unsigned int i = 0; i < sizeof(str) - 1; i++) \
    int_10h_0eh(str[i])

static const char notification_string[] = "Boot main OK!\r\n";

void main(void)
{
    MSG(notification_string);
}

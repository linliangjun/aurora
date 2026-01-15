/**
 * 用户任务
 *
 * Copyright 2026 linliangjun
 */

#include "syscall.h"
#include "types.h"

/**
 * 用户任务入口
 */
void user_task(void)
{
    sys_write("User task\n");
    while (1)
        ;
}

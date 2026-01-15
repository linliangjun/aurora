/**
 * 任务管理器
 *
 * Copyright 2025 linliangjun
 */

#ifndef __TASK_MANAGER_H
#define __TASK_MANAGER_H

#include "types.h"

/**
 * 初始化任务管理器
 */
void task_manager_init(void);

/**
 * 创建任务
 *
 * @param entry_point 任务入口点
 * @param user 是否为用户态任务，true:用户态；false:内核态
 * @return 任务索引
 */
size_t task_spawn(uintptr_t entry_point, bool user);

/**
 * 调度任务
 */
void task_manager_schedule(void);

#endif

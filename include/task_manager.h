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
 * @return 任务索引
 */
size_t task_spawn(uintptr_t entry_point);

/**
 * 调度任务
 */
void task_manager_schedule(void);

#endif

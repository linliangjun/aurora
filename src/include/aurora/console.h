/**
 * 控制台驱动
 *
 * Copyright 2024 linliangjun
 */

#ifndef AURORA_CONSOLE_H
#define AURORA_CONSOLE_H

#include <stdint.h>
#include <stdbool.h>

/**
 * 获取光标位置
 *
 * @return 屏幕位置，范围在 [0xb8000 ~ 0xb8fa0) 之间
 */
int8_t *console_cursor_get(void);

/**
 * 打印字符串
 *
 * @param ptr 屏幕位置，范围应该在 [0xb8000 ~ 0xb8fa0) 之间
 * @param str 字符串指针
 * @param new_line 是否另起一行
 * @param update_cursor 是否更新光标位置
 *
 * @return 更新后的屏幕位置
 */
int8_t *console_write(int8_t *ptr, const int8_t *str, bool new_line, bool update_cursor);

#endif // AURORA_CONSOLE_H

/**
 * 控制台驱动
 *
 * Copyright 2025 linliangjun
 */

#ifndef __AURORA_CONSOLE_H
#define __AURORA_CONSOLE_H

#include "aurora/types.h"

/**
 * 设置光标位置
 *
 * @param video_text_ptr 屏幕位置，范围在 [0xb8000 ~ 0xb8fa0) 之间
 */
void set_cursor(const char *video_text_ptr);

/**
 * 获取光标位置
 *
 * @return 屏幕位置，范围在 [0xb8000 ~ 0xb8fa0) 之间
 */
char *get_cursor(void);

/**
 * 设置字符属性
 *
 * @param attr 字符属性
 */
void set_char_attr(u8 attr);

/**
 * 获取字符属性
 *
 * @return 字符属性
 */
u8 get_char_attr(void);

/**
 * 初始化控制台
 */
void console_init(void);

/**
 * 清空控制台
 */
void console_clean(void);

/**
 * 打印字符串
 *
 * @param video_text_ptr 屏幕位置，范围在 [0xb8000 ~ 0xb8fa0) 之间
 * @param str 字符串
 */
char *console_write(char *video_text_ptr, const char *str);

#endif

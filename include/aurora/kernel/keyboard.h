/**
 * 键盘驱动
 *
 * Copyright 2026 linliangjun
 */

#ifndef __AURORA_KERNEL_KEYBOARD_H
#define __AURORA_KERNEL_KEYBOARD_H

#include "aurora/types.h"

/**
 * 初始化键盘驱动
 */
void keyboard_init(void);

/**
 * 获取键盘扫描码，0 或负数表示缓冲区为空
 *
 * @return 键盘扫描码
 */
i16 keyboard_get_scancode(void);

#endif // __AURORA_KERNEL_KEYBOARD_H

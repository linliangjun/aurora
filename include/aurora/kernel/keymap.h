/**
 * 键盘映射表
 *
 * Copyright 2026 linliangjun
 */

#ifndef __AURORA_KERNEL_KEYMAP_H
#define __AURORA_KERNEL_KEYMAP_H

#include "aurora/types.h"

/**
 * 将键盘扫描码转换为 ASCII 字符
 *
 * @param scancode 键盘扫描码
 * @return 对应的 ASCII 字符，如果是特殊键或无对应字符则返回负数
 */
i16 keymap_translate(u8 scancode);

#endif // __AURORA_KERNEL_KEYMAP_H

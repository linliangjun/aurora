/**
 * 键盘映射表实现
 *
 * Copyright 2026 linliangjun
 */

#include "keymap.h"
#include "ctype.h"

// 键盘扫描码映射表 (US QWERTY)
// 索引是 Make Code
static char keymap[128] = {
    0, 27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',     /* 0x00 - 0x0E */
    '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',      /* 0x0F - 0x1C */
    0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',              /* 0x1D - 0x29 */
    0, '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0,                /* 0x2A - 0x35 */
    '*', 0, ' ', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,                                   /* 0x36 - 0x44 */
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '-', 0, 0, 0, '+', 0,                          /* 0x45 - 0x54 */
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0                                           /* 0x55 - 0x60 */
};

// Shift 状态下的映射表 (仅特殊符号)
// 对于字母，直接 -32 即可，不需要查表
// 索引是 Make Code
static char keymap_shift[128] = {
    0, 27, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '\b',     /* 0x00 - 0x0E */
    '\t', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n',      /* 0x0F - 0x1C */
    0, 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '"', '~',               /* 0x1D - 0x29 */
    0, '|', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?', 0,                 /* 0x2A - 0x35 */
    '*', 0, ' ', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,                                   /* 0x36 - 0x44 */
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '-', 0, 0, 0, '+', 0,                          /* 0x45 - 0x54 */
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0                                           /* 0x55 - 0x60 */
};

static bool shift_pressed = false;

i16 keymap_translate(u8 scancode)
{
    // 1. 处理按下事件
    if (!(scancode & 0x80))
    {
        // 记录 Shift 状态
        if (scancode == 0x2A || scancode == 0x36) // 左 Shift 或 右 Shift
        {
            shift_pressed = true;
            return -1;
        }

        // 查表
        if (scancode > 0 && scancode < 128)
            return shift_pressed ? keymap_shift[scancode] : keymap[scancode];
    }
    // 2. 处理松开事件
    else
    {
        u8 make_code = scancode & 0x7F;
        if (make_code == 0x2A || make_code == 0x36)
            shift_pressed = false;
    }
    return -1;
}

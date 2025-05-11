/**
 * 基于 CGA 文本模式（80 * 25，16 色）的 TTY 驱动
 *
 * 与 BIOS 中断（int 10h）不同的点在于：换行符使用 '\n' 而不是 "\r\n"。
 *
 * Copyright 2025 linliangjun
 */

#include "tty.h"
#include "io.h"
#include "string.h"

/* CGA 文本颜色 */
typedef enum
{
    BLACK,         // 黑色
    BLUE,          // 蓝色
    GREEN,         // 绿色
    CYAN,          // 青色
    RED,           // 红色
    MAGENTA,       // 紫色
    BROWN,         // 棕色
    LIGHT_GRAY,    // 浅灰色
    DARK_GRAY,     // 深灰色
    LIGHT_BLUE,    // 浅蓝色
    LIGHT_GREEN,   // 浅绿色
    LIGHT_CYAN,    // 浅青色
    LIGHT_RED,     // 浅红色
    LIGHT_MAGENTA, // 浅紫色
    LIGHT_YELLOW,  // 浅黄色
    WHITE,         // 白色
} cga_text_color_t;

/* CGA 文本属性 */
typedef struct __attribute__((packed))
{
    cga_text_color_t fg_color : 4; // 前景色
    cga_text_color_t bg_color : 4; // 背景色
} cga_text_attr_t;

typedef struct __attribute__((packed))
{
    char ch;
    cga_text_attr_t attr;
} cga_text_unit_t;

/* 屏幕位置，从 0 开始计数 */
typedef struct
{
    u8 x;
    u8 y;
} screen_pos_t;

#define CGA_TEXT_FRAME_BUFFER_ADDR 0xb8000 // CGA 文本模式显存地址
#define SCREEN_WIDTH 80                    // 屏幕宽度（单位：字符）
#define SCREEN_HEIGHT 25                   // 屏幕高度（单位：字符）

#define CRT_INDEX_PORT 0x3d4 /* CRT 索引端口 */
#define CRT_DATA_PORT 0x3d5  /* CRT 数据端口 */

#define CURSOR_ADDR_HIGH_PORT 0xe /* 光标地址端口（高 8 位） */
#define CURSOR_ADDR_LOW_PORT 0xf  /* 光标地址端口（低 8 位） */

static const cga_text_attr_t default_text_attr = {.bg_color = BLACK, .fg_color = LIGHT_GRAY};
static cga_text_unit_t (*const frame_buf)[SCREEN_WIDTH] =
    (cga_text_unit_t (*)[SCREEN_WIDTH])CGA_TEXT_FRAME_BUFFER_ADDR;

static void set_cursor_pos(const screen_pos_t *pos)
{
    u16 _pos = pos->y * SCREEN_WIDTH + pos->x;
    outb(CURSOR_ADDR_LOW_PORT, CRT_INDEX_PORT);
    outb(_pos, CRT_DATA_PORT);
    outb(CURSOR_ADDR_HIGH_PORT, CRT_INDEX_PORT);
    outb(_pos >> 8, CRT_DATA_PORT);
}

static screen_pos_t get_cursor_pos(void)
{
    u16 _pos;
    outb(CURSOR_ADDR_LOW_PORT, CRT_INDEX_PORT);
    _pos = inb(CRT_DATA_PORT);
    outb(CURSOR_ADDR_HIGH_PORT, CRT_INDEX_PORT);
    _pos |= inb(CRT_DATA_PORT) << 8;
    screen_pos_t pos = {.x = _pos % SCREEN_WIDTH, .y = _pos / SCREEN_WIDTH};
    return pos;
}

static void scroll_up(screen_pos_t *pos)
{
    if (pos->y < SCREEN_HEIGHT)
        return;
    pos->y--;

    // 上移一行
    memcpy(frame_buf, frame_buf[1],
           (SCREEN_HEIGHT - 1) * SCREEN_WIDTH * sizeof(cga_text_unit_t));

    // 清空最后一行
    cga_text_unit_t blank = {.ch = ' ', .attr = default_text_attr};
    for (size_t i = 0; i < SCREEN_WIDTH; i++)
        frame_buf[SCREEN_HEIGHT - 1][i] = blank;
}

void tty_write(const char *msg)
{
    screen_pos_t pos = get_cursor_pos();
    char ch;
    while ((ch = *msg++))
    {
        switch (ch)
        {
        case '\n':
            pos.x = 0;
            pos.y++;
            scroll_up(&pos);
            break;
        case '\b':
            if (pos.x)
                pos.x--;
            else if (pos.y)
            {
                pos.x = SCREEN_WIDTH - 1;
                pos.y--;
            }
            break;
        default:
            cga_text_unit_t unit = {.ch = ch, .attr = default_text_attr};
            frame_buf[pos.y][pos.x] = unit;
            if (pos.x < SCREEN_WIDTH - 1)
                pos.x++;
            else
            {
                pos.x = 0;
                pos.y++;
                scroll_up(&pos);
            }
        }
    }
    set_cursor_pos(&pos);
}

void tty_printf(const char *fmt, ...)
{
    char buf[1024];

    va_list args;
    va_start(args, fmt);
    vsprintf(buf, fmt, args);
    va_end(args);

    tty_write(buf);
}

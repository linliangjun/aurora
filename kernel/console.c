/**
 * 控制台驱动
 *
 * Copyright 2025 linliangjun
 */

#include "aurora/console.h"
#include "aurora/io.h"
#include "string.h"

#define VIDEO_INDEX_PORT 0x3d4    /* 显卡寄存器索引端口地址端口 */
#define VIDEO_DATA_PORT 0x3d5     /* 显卡寄存器数据端口 */
#define CURSOR_ADDR_HIGH_PORT 0xe /* 光标地址端口（低 8 位） */
#define CURSOR_ADDR_LOW_PORT 0xf  /* 光标地址端口（高 8 位） */

#define VIDEO_TEXT_BASE_ADDR 0xb8000 /* 文本显存基地址 */
#define VIDEO_TEXT_LINE_SIZE 160     /* 文本显存行大小 */
#define VIDEO_TEXT_PAGE_SIZE 4000    /* 文本显存页大小 */

#define ALIGN_LINE(x) (VIDEO_TEXT_LINE_SIZE - ((x) - VIDEO_TEXT_BASE_ADDR) % VIDEO_TEXT_LINE_SIZE)

static u8 _char_attr;
static char *_video_text_ptr;

void set_cursor(const char *video_text_ptr)
{
    u16 pos = (u16)((u32)video_text_ptr - VIDEO_TEXT_BASE_ADDR) >> 1;
    outb(CURSOR_ADDR_LOW_PORT, VIDEO_INDEX_PORT);
    outb(pos, VIDEO_DATA_PORT);
    outb(CURSOR_ADDR_HIGH_PORT, VIDEO_INDEX_PORT);
    outb(pos >> 8, VIDEO_DATA_PORT);
    _video_text_ptr = (char *)video_text_ptr;
}

char *get_cursor(void)
{
    return _video_text_ptr;
}

void set_char_attr(u8 attr)
{
    _char_attr = attr;
}

u8 get_char_attr(void)
{
    return _char_attr;
}

void console_init(void)
{
    _char_attr = 7;
    console_clean();
}

void console_clean(void)
{
    char *video_text_ptr = (char *)VIDEO_TEXT_BASE_ADDR;
    set_cursor(video_text_ptr);
    u32 *ptr = (u32 *)video_text_ptr;
    for (u32 i = 0; i < VIDEO_TEXT_PAGE_SIZE >> 2; i++)
        *ptr++ = 0x07000700;
}

static char *scroll_up(char *video_text_ptr)
{
    if ((u32)video_text_ptr >= VIDEO_TEXT_BASE_ADDR + VIDEO_TEXT_PAGE_SIZE)
    {
        video_text_ptr -= VIDEO_TEXT_LINE_SIZE;
        memcpy(
            (char *)VIDEO_TEXT_BASE_ADDR,
            (char *)(VIDEO_TEXT_BASE_ADDR + VIDEO_TEXT_LINE_SIZE),
            VIDEO_TEXT_PAGE_SIZE - VIDEO_TEXT_LINE_SIZE);
        u32 *ptr = (u32 *)(VIDEO_TEXT_BASE_ADDR + VIDEO_TEXT_PAGE_SIZE - VIDEO_TEXT_LINE_SIZE);
        for (u32 i = 0; i < VIDEO_TEXT_LINE_SIZE >> 2; i++)
            *ptr++ = 0x07000700;
    }
    return video_text_ptr;
}

char *console_write(char *video_text_ptr, const char *str)
{
    char ch;
    while ((ch = *str++))
    {
        switch (ch)
        {
        case '\n':
            video_text_ptr += ALIGN_LINE((u32)video_text_ptr);
            video_text_ptr = scroll_up(video_text_ptr);
            break;
        case '\b':
            if ((u32)video_text_ptr >= VIDEO_TEXT_BASE_ADDR + 2)
                video_text_ptr -= 2;
            break;
        default:
            *video_text_ptr++ = ch;
            *video_text_ptr++ = _char_attr;
            video_text_ptr = scroll_up(video_text_ptr);
            break;
        }
    }
    return video_text_ptr;
}

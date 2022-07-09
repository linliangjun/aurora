#include <io.h>
#include <console.h>

#define _SCREEN_WIDTH 80        /* 屏幕宽度 */
#define _SCREEN_HEIGHT 25       /* 屏幕高度 */
#define _VIDEO_ADDR_PORT 0x3d4  /* 显卡地址端口 */
#define _VIDEO_VALUE_PORT 0x3d5 /* 显卡值端口 */
#define _CURSOR_ADDRH_PORT 0xe  /* 光标地址端口（低 8 位） */
#define _CURSOR_ADDRL_PORT 0xf  /* 光标地址端口（高 8 位） */

void set_cursor_pos(unsigned char x, unsigned char y)
{
    if (x >= _SCREEN_WIDTH || y >= _SCREEN_HEIGHT)
    {
        return;
    }
    unsigned short pos = y * _SCREEN_WIDTH + x;
    outb(_VIDEO_ADDR_PORT, _CURSOR_ADDRL_PORT);
    outb(_VIDEO_VALUE_PORT, pos);
    outb(_VIDEO_ADDR_PORT, _CURSOR_ADDRH_PORT);
    outb(_VIDEO_VALUE_PORT, pos >> 8);
}

console_point get_cursor_pos()
{
    struct console_point point;
    outb(_VIDEO_ADDR_PORT, _CURSOR_ADDRL_PORT);
    unsigned short pos = inb(_VIDEO_VALUE_PORT);
    outb(_VIDEO_ADDR_PORT, _CURSOR_ADDRH_PORT);
    pos = (inb(_VIDEO_VALUE_PORT) << 8) | pos;
    point.x = pos % _SCREEN_WIDTH;
    point.y = pos / _SCREEN_WIDTH;
    return point;
}

#include <io.h>
#include <console.h>

#define _SCREEN_WIDTH 80        /* 屏幕宽度 */
#define _SCREEN_HEIGHT 25       /* 屏幕高度 */
#define _VIDEO_ADDR_PORT 0x3d4  /* 显卡地址端口 */
#define _VIDEO_VALUE_PORT 0x3d5 /* 显卡值端口 */
#define _CURSOR_ADDRH_PORT 0xe  /* 光标地址端口（低 8 位） */
#define _CURSOR_ADDRL_PORT 0xf  /* 光标地址端口（高 8 位） */

typedef struct _console_cursor_point
{
    unsigned char x;
    unsigned char y;
} _console_cursor_point;

static void set_console_cursor_point(unsigned char x, unsigned char y)
{
    unsigned short pos = y * _SCREEN_WIDTH + x;
    outb(_VIDEO_ADDR_PORT, _CURSOR_ADDRL_PORT);
    outb(_VIDEO_VALUE_PORT, pos);
    outb(_VIDEO_ADDR_PORT, _CURSOR_ADDRH_PORT);
    outb(_VIDEO_VALUE_PORT, pos >> 8);
}

static _console_cursor_point get_console_cursor_point()
{
    struct _console_cursor_point point;
    outb(_VIDEO_ADDR_PORT, _CURSOR_ADDRL_PORT);
    unsigned short pos = inb(_VIDEO_VALUE_PORT);
    outb(_VIDEO_ADDR_PORT, _CURSOR_ADDRH_PORT);
    pos = (inb(_VIDEO_VALUE_PORT) << 8) | pos;
    point.x = pos % _SCREEN_WIDTH;
    point.y = pos / _SCREEN_WIDTH;
    return point;
}

static unsigned char global_attr;
static unsigned char attr;

static unsigned char gen_attr(_Bool blinking, _Bool highlight, unsigned char bg, unsigned char fg)
{
    return (((bg & 7) | ((blinking << 3) & 8)) << 4) | ((fg & 7) | ((highlight << 3) & 8));
}

void set_console_attr(_Bool blinking, _Bool highlight, unsigned char bg, unsigned char fg)
{
    attr = gen_attr(blinking, highlight, bg, fg);
}

void console_init(_Bool blinking, _Bool highlight, unsigned char bg, unsigned char fg)
{
    global_attr = gen_attr(blinking, highlight, bg, fg);
    set_console_attr(blinking, highlight, bg, fg);
    console_clean();
}

#define _DISPLAY_MEM_BADDR 0xb8000 /* 显存基地址 */

void console_clean()
{
    unsigned short *ptr =(unsigned short*)_DISPLAY_MEM_BADDR;
    for (int i = 0; i < _SCREEN_HEIGHT * _SCREEN_WIDTH; i++)
    {
        ptr[i] = global_attr << 8;
    }
    set_console_cursor_point(0, 0);
}

#define _ASCII_LF 0xa
#define _ASCII_CR 0xd

static void scroll_up(_console_cursor_point *point)
{
    unsigned char Y = (*point).y;
    unsigned short *ptr = (unsigned short *)_DISPLAY_MEM_BADDR;
    if (Y == 0)
    {
        for (unsigned char x = 0; x < _SCREEN_WIDTH; x++)
        {
            ptr[x] = global_attr << 8;
        }
        (*point).x = 0;
    }
    else
    {
        for (unsigned char y = 1; y <= Y; y++)
        {
            for (unsigned char x = 0; x < _SCREEN_WIDTH; x++)
            {
                ptr[x + (y - 1) * 80] = ptr[x + y * 80];
            }
            if (y == Y)
            {
                for (unsigned char x = 0; x < _SCREEN_WIDTH; x++)
                {
                    ptr[x + y * 80] = global_attr << 8;
                }
            }
        }
    }
}

static void lf(_console_cursor_point *point)
{
    if ((*point).y + 1 >= _SCREEN_HEIGHT)
    {
        scroll_up(point);
    }
    else
    {
        (*point).y += 1;
    }
}

static void cr(_console_cursor_point *point)
{
    (*point).x = 0;
}

void console_print(char *msg, unsigned int len)
{
    _console_cursor_point point = get_console_cursor_point();
    unsigned short *ptr = (unsigned short *)((point.x + point.y * _SCREEN_WIDTH) * 2 + _DISPLAY_MEM_BADDR);
    for (unsigned int i = 0; i < len; i++)
    {
        char ch = msg[i];
        switch (ch)
        {
        case _ASCII_LF:
            lf(&point);
            ptr = (unsigned short *)((point.x + point.y * _SCREEN_WIDTH) * 2 + _DISPLAY_MEM_BADDR);
            break;
        case _ASCII_CR:
            cr(&point);
            ptr = (unsigned short *)((point.x + point.y * _SCREEN_WIDTH) * 2 + _DISPLAY_MEM_BADDR);
            break;
        default:
            if (point.x + 1 > _SCREEN_WIDTH)
            {
                lf(&point);
                cr(&point);
                ptr = (unsigned short *)((point.x + point.y * _SCREEN_WIDTH) * 2 + _DISPLAY_MEM_BADDR);
            }
            *ptr = (attr << 8) | ch;
            ptr++;
            point.x++;

            break;
        }
    }
    set_console_cursor_point(point.x, point.y);
}

typedef struct console_point
{
    unsigned char x;
    unsigned char y;
} console_point;

void set_cursor_pos(unsigned char x, unsigned char y);

console_point get_cursor_pos();

#define CONSOLE_ATTR_BLACK 0
#define CONSOLE_ATTR_BLUE 1
#define CONSOLE_ATTR_GREEN 2
#define CONSOLE_ATTR_CYAN 3
#define CONSOLE_ATTR_RED 4
#define CONSOLE_ATTR_MAGENTA 5
#define CONSOLE_ATTR_BROWN 6
#define CONSOLE_ATTR_GRAY 7

void set_console_global_attr(_Bool blinking, _Bool highlight, unsigned char bg, unsigned char fg);

void set_console_attr(_Bool blinking, _Bool highlight, unsigned char bg, unsigned char fg);

void console_clean();

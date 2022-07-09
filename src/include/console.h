typedef struct console_point
{
    unsigned char x;
    unsigned char y;
} console_point;

void set_cursor_pos(unsigned char x, unsigned char y);

console_point get_cursor_pos();

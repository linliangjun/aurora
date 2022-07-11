#define CONSOLE_ATTR_BLACK 0
#define CONSOLE_ATTR_BLUE 1
#define CONSOLE_ATTR_GREEN 2
#define CONSOLE_ATTR_CYAN 3
#define CONSOLE_ATTR_RED 4
#define CONSOLE_ATTR_MAGENTA 5
#define CONSOLE_ATTR_BROWN 6
#define CONSOLE_ATTR_GRAY 7

void console_init(_Bool blinking, _Bool highlight, unsigned char bg, unsigned char fg);

void set_console_attr(_Bool blinking, _Bool highlight, unsigned char bg, unsigned char fg);

void console_clean();

void console_print(char *msg, unsigned int len);

#include <console.h>

char msg[] = "Kernel initializing...";

void kernel_init()
{
    console_init(0, 1, CONSOLE_ATTR_CYAN, CONSOLE_ATTR_GRAY);
    set_console_attr(0, 1, CONSOLE_ATTR_BLACK, CONSOLE_ATTR_GRAY);
    console_print(msg, sizeof(msg) - 1);    
}

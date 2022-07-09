#include <console.h>

void kernel_init()
{
    set_console_global_attr(0, 1, CONSOLE_ATTR_CYAN, CONSOLE_ATTR_GRAY);
    console_clean();
    char msg[] = "Kernel initializing...";
    char *ptr = (char *)(0xb8000);
    for (int i = 0; i < sizeof(msg); i++)
    {
        ptr[i * 2] = msg[i];
    }

    // 设置光标位置
    set_cursor_pos(sizeof(msg) - 1, 0);
}

#include <console.h>

void kernel_init()
{
    char msg[] = "Kernel initializing...";
    char *ptr = (char *)(0xb8000 + 480);
    for (int i = 0; i < sizeof(msg); i++)
    {
        ptr[i * 2] = msg[i];
    }

    // 设置光标位置
    set_cursor_pos(sizeof(msg) - 1, 3);
}

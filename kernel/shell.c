/**
 * Shell
 *
 * Copyright 2026 linliangjun
 */

#include "shell.h"
#include "keymap.h"
#include "keyboard.h"
#include "kernel.h"
#include "tty.h"
#include "printk.h"
#include "task_manager.h"

static char cmd_buf[SHELL_CMD_BUFFER_SIZE];
static size_t cmd_len = 0;

static void char_handler(char c)
{
    char buf[2] = {0, 0};
    switch (c)
    {
    case '\n':
        tty_write("\n# ");
        // 在这里处理命令
        cmd_len = 0;
        break;
    case '\b':
        if (cmd_len > 0)
        {
            tty_write("\b \b");
            cmd_len--;
        }
        break;
    default:
        buf[0] = c;
        tty_write(buf);
        cmd_buf[cmd_len++] = c;
    }
}

void shell_main(void)
{
    printk("Kernel version \"%s\" (build %s %s)\n", KERNEL_VERSION, __DATE__, __TIME__);
    tty_write("# ");
    while (true)
    {
        i16 scancode;
        do
        {
            scancode = keyboard_get_scancode();
            i16 c = keymap_translate(scancode);
            if (c >= 0)
                char_handler(c);
        } while (scancode > 0);
        task_manager_schedule();
    }
}

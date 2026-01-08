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
#include "task_manager.h"
#include "ramfs.h"
#include "string.h"
#include "assert.h"

static char cwd[SHELL_CWD_BUFFER_SIZE] = "/";
static char cmd_buf[SHELL_CMD_BUFFER_SIZE];
static size_t cmd_len = 0;

static bool ls_filter(ramfs_entry_t *entry)
{
    return strncmp(entry->name, cwd, strlen(cwd)) == 0;
}

static void ls_printer(ramfs_entry_t *entry)
{
    char *name = entry->name;
    name += strlen(cwd);
    size_t name_len = strlen(name);
    if (name_len != 0)
    {
        char *index = strchr(name, '/');
        if (index == 0 || (uintptr_t) index - (uintptr_t) name == name_len - 1)
        {
            printk("%s    ", name);
        }
    }
}

static void cmd_ls_handler(void)
{
    ramfs_ls(ls_filter, ls_printer);
    tty_write("\n# ");
}

static void char_handler(char c)
{
    char buf[2] = {0, 0};
    switch (c)
    {
    case '\n':
        tty_write("\n");
        cmd_buf[cmd_len] = '\0';
        if (strcmp("ls", cmd_buf) == 0)
            cmd_ls_handler();
        else
            printk("%s: command not found\n# ", cmd_buf);
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
        ASSERT(cmd_len < SHELL_CMD_BUFFER_SIZE, "command buffer overflow");
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

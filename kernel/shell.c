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
#include "ramfs.h"
#include "string.h"
#include "assert.h"
#include "heap.h"

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

static void cmd_cat_handler(void)
{
    char *name = cmd_buf + 4;
    char *path = name;
    bool clear_heap = false;
    if (*name != '/')
    {
        size_t cwd_len = strlen(cwd);
        size_t path_len = cwd_len + strlen(name) + 1;
        bool require_separator = false;
        if (cwd[cwd_len - 1] != '/')
        {
            path_len += 1;
            require_separator = true;
        }
        path = heap_malloc(path_len, false);
        strcpy(path, cwd);
        if (require_separator)
            strcat(path, "/");
        strcat(path, name);
        clear_heap = true;
    }
    ramfs_file_t file = ramfs_open(path);
    if (file.start_addr == 0)
    {
        if (file.is_dir)
            printk("cat: %s: Is a directory\n", name);
        else
            printk("cat: %s: No such file or directory\n", name);
    }
    else
    {
        size_t remaining_size = file.size - file.offset;
        if (remaining_size > 0)
        {
            size_t buf_size = MIN(SHELL_CAT_BUFFER_SIZE, remaining_size);
            char *buf = heap_malloc(buf_size, false);
            while (remaining_size > 0)
            {
                size_t read_size = ramfs_read(&file, buf, buf_size - 1);
                buf[read_size] = '\0';
                tty_write(buf);
                remaining_size = file.size - file.offset;
            }
            heap_free(buf);
        }
    }
    if (clear_heap)
        heap_free(path);
    tty_write("# ");
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
        else if(strncmp("cat ", cmd_buf, 4) == 0)
            cmd_cat_handler();
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
    }
}

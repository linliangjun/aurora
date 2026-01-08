/**
 * 内存文件系统
 *
 * Copyright 2026 linliangjun
 */

#include "ramfs.h"
#include "types.h"
#include "printk.h"
#include "string.h"
#include "math.h"

typedef struct {
    char name[100];     // 文件名
    char mode[8];       // 权限 (八进制)
    char uid[8];        // 用户ID (八进制)
    char gid[8];        // 组ID (八进制)
    char size[12];      // 文件大小 (八进制字符串)
    char mtime[12];     // 修改时间 (八进制)
    char chksum[8];     // 校验和 (八进制)
    char typeflag;      // 文件类型 ('0'=文件, '5'=目录)
    char linkname[100]; // 链接目标
    char magic[6];      // "ustar" 魔数
    char version[2];    // 版本
    char uname[32];     // 用户名
    char gname[32];     // 组名
    char devmajor[8];   // 设备主号
    char devminor[8];   // 设备次号
    char prefix[155];   // 文件名前缀 (用于长路径)
    char padding[12];   // 填充至 512 字节
} __attribute__((packed)) tar_header_t;

static uintptr_t ramfs_start;
static size_t ramfs_size;

// 将八进制字符串转换为整数
static size_t oct2bin(const char *str, int size)
{
    size_t n = 0;
    const char *c = str;
    while (size-- > 0 && *c >= '0' && *c <= '7') {
        n = (n * 8) + (*c - '0');
        c++;
    }
    return n;
}

void ramfs_init(uintptr_t start, size_t size)
{
    ramfs_start = start;
    ramfs_size = size;
    PR_INFO("RAMFS at %#x, size: %d\n", start, size);
}

void ramfs_ls(ramfs_entry_filter filter ,ramfs_entry_handler handler)
{
    uintptr_t current_addr = ramfs_start;
    while (current_addr < ramfs_start + ramfs_size) {
        tar_header_t *header = (tar_header_t *)current_addr;

        // 检查文件名是否为空，为空则表示结束
        if (header->name[0] == '\0')
            break;

        // 获取文件大小
        size_t file_size = oct2bin(header->size, 11);

        // 构造 entry
        ramfs_entry_t entry;
        // 确保 name 以 null 结尾
        // 处理文件名: 如果以 ./ 开头，去掉它
        char *name_ptr = header->name;
        if (name_ptr[0] == '.' && name_ptr[1] == '/') {
            name_ptr += 2;
        }

        // 确保 name 以 / 开头
        entry.name[0] = '/';
        size_t name_len = strlen(name_ptr);
        if (name_len > sizeof(entry.name) - 2) {
             name_len = sizeof(entry.name) - 2;
        }
        memcpy(entry.name + 1, name_ptr, name_len);
        entry.name[name_len + 1] = '\0';

        entry.size = file_size;
        entry.is_dir = (header->typeflag == '5');

        // 过滤并处理
        if (!filter || filter(&entry)) {
            if (handler) {
                handler(&entry);
            }
        }

        // 移动到下一个头部
        // 头部大小(512) + 文件内容大小(按512对齐)
        size_t offset = 512 + ((file_size + 511) & ~511);
        current_addr += offset;
    }
}

ramfs_file_t ramfs_open(const char *path)
{
    ramfs_file_t file = {.start_addr = 0};
    uintptr_t current_addr = ramfs_start;
    while (current_addr < ramfs_start + ramfs_size) {
        tar_header_t *header = (tar_header_t *)current_addr;

        // 检查文件名是否为空，为空则表示结束
        if (header->name[0] == '\0')
            break;

        size_t file_size = oct2bin(header->size, 11);

        // 处理文件名: 如果以 ./ 开头，去掉它
        char *name_ptr = header->name;
        if (name_ptr[0] == '.' && name_ptr[1] == '/') {
            name_ptr += 2;
        }

        if(*path == '/')
            path++;

        if(strcmp(path, name_ptr) == 0)
        {
            file.is_dir = header->typeflag == '5';
            file.start_addr = (uintptr_t)(header + 1);
            file.size = file_size;
            file.offset = 0;
            break;
        }

        // 移动到下一个头部
        // 头部大小(512) + 文件内容大小(按512对齐)
        size_t offset = 512 + ((file_size + 511) & ~511);
        current_addr += offset;
    }
    return file;
}

size_t ramfs_read(ramfs_file_t *file, void *buf, size_t size)
{
    if (file->start_addr == 0 || file->is_dir)
        return 0;
    size_t remaining_size = file->size - file->offset;
    size_t _size = MIN(remaining_size, size);
    if (_size == 0)
        return 0;
    memcpy(buf, (void *)(file->start_addr + file->offset), _size);
    file->offset += _size;
    return _size;
}

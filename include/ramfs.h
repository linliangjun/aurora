/**
 * 内存文件系统
 *
 * Copyright 2026 linliangjun
 */

#ifndef __RAMFS_H
#define __RAMFS_H

#include "types.h"

typedef struct {
    char name[100];
    size_t size;
    bool is_dir;
} ramfs_entry_t;

typedef bool (*ramfs_entry_filter)(ramfs_entry_t *entry);

typedef void (*ramfs_entry_handler)(ramfs_entry_t *entry);

/**
 * 初始化内存文件系统
 *
 * @param start 内存文件系统起始地址
 * @param size  内存文件系统大小
 */
void ramfs_init(uintptr_t start, size_t size);

/**
 * 列出内存文件系统中的文件
 *
 * @param filter 过滤器
 * @param handler 处理器
 */
void ramfs_ls(ramfs_entry_filter filter ,ramfs_entry_handler handler);

#endif // __RAMFS_H

/**
 * 内存文件系统
 *
 * Copyright 2026 linliangjun
 */

#ifndef __AURORA_KERNEL_RAMFS_H
#define __AURORA_KERNEL_RAMFS_H

#include "aurora/types.h"

typedef struct {
    char name[100];
    size_t size;
    bool is_dir;
} ramfs_entry_t;

typedef struct {
    uintptr_t start_addr; // 文件数据在内存中的起始地址，0 表示不存在
    size_t size;          // 文件总大小
    bool is_dir;          // 是否是目录
    size_t offset;        // 当前读到了哪里 (游标)
} ramfs_file_t;

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

/**
 * 打开文件
 *
 * @param path 文件路径
 * @return 文件句柄
 */
ramfs_file_t ramfs_open(const char *path);

/**
 * 读取文件
 *
 * @param file 文件句柄
 * @param buf  读取到的内容存放缓冲区
 * @param size 要读取的大小
 * @return 实际读取到的大小
 */
size_t ramfs_read(ramfs_file_t *file, void *buf, size_t size);


#endif // __AURORA_KERNEL_RAMFS_H

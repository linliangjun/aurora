/**
 * ELF 加载器
 *
 * Copyright 2026 linliangjun
 */

#ifndef __ELF_LOADER_H
#define __ELF_LOADER_H

#include "types.h"

#define ELF_LOAD_ERROR_FILE_NOT_FOUND -1
#define ELF_LOAD_ERROR_IS_DIRECTORY   -2
#define ELF_LOAD_ERROR_INVALID_FORMAT -3

/**
 * 加载 ELF 文件
 *
 * @param path ELF 文件路径
 * @param entry 指向 ELF 文件入口地址的指针
 *
 * @return 错误码，0 表示成功
 */
i32 load_elf(const char *path, uintptr_t *entry);

#endif

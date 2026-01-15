/**
 * ELF 32 文件
 *
 * Copyright 2025 linliangjun
 */

#ifndef __AURORA_ELF32_H
#define __AURORA_ELF32_H

#include "aurora/types.h"

#define EI_NIDENT 16

#define PT_LOAD 1 // 可加载段

/* ELF32 文件头 */
typedef struct
{
    u8 e_ident[EI_NIDENT]; // 魔数和相关信息
    u16 e_type;            // 目标文件类型
    u16 e_machine;         // 硬件体系
    u16 e_version;         // 目标文件版本
    u32 e_entry;             // 程序进入点
    u32 e_phoff;             // 程序头偏移量
    u32 e_shoff;             // 节头偏移量
    u32 e_flags;             // 处理器特定标志
    u16 e_ehsize;          // ELF头部长度
    u16 e_phentsize;       // 程序头一个条目的长度
    u16 e_phnum;           // 程序头条目个数
    u16 e_shentsize;       // 节头一个条目的长度
    u16 e_shnum;           // 节头条目个数
    u16 e_shstrndx;        // 节头字符表索引
} elf32_ehdr_t;

/* ELF32 程序头 */
typedef struct
{
    u32 p_type;   // 段类型
    u32 p_offset; // 段位置相对于文件开始处的偏移量
    u32 p_vaddr;  // 段在内存中的地址
    u32 p_paddr;  // 段的物理地址
    u32 p_filesz; // 段在文件中的长度
    u32 p_memsz;  // 段在内存中的长度
    u32 p_flags;  // 段的标记
    u32 p_align;  // 段在内存中对齐标记
} elf32_phdr_t;

#endif // __AURORA_ELF32_H

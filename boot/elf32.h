/**
 * ELF 32 文件
 *
 * Copyright 2024 linliangjun
 */

#ifndef __ELF32_H
#define __ELF32_H

#define EI_NIDENT 16

#define PT_LOAD 1   // 可加载段

typedef struct
{
    unsigned char e_ident[EI_NIDENT]; // 魔数和相关信息
    unsigned short e_type;            // 目标文件类型
    unsigned short e_machine;         // 硬件体系
    unsigned short e_version;         // 目标文件版本
    unsigned int e_entry;             // 程序进入点
    unsigned int e_phoff;             // 程序头偏移量
    unsigned int e_shoff;             // 节头偏移量
    unsigned int e_flags;             // 处理器特定标志
    unsigned short e_ehsize;          // ELF头部长度
    unsigned short e_phentsize;       // 程序头一个条目的长度
    unsigned short e_phnum;           // 程序头条目个数
    unsigned short e_shentsize;       // 节头一个条目的长度
    unsigned short e_shnum;           // 节头条目个数
    unsigned short e_shstrndx;        // 节头字符表索引
} elf32_ehdr_t;

typedef struct
{
    unsigned int p_type;   // 段类型
    unsigned int p_offset; // 段位置相对于文件开始处的偏移量
    unsigned int p_vaddr;  // 段在内存中的地址
    unsigned int p_paddr;  // 段的物理地址
    unsigned int p_filesz; // 段在文件中的长度
    unsigned int p_memsz;  // 段在内存中的长度
    unsigned int p_flags;  // 段的标记
    unsigned int p_align;  // 段在内存中对齐标记
} elf32_phdr_t;

#endif

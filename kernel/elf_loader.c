/**
 * ELF 加载器
 *
 * Copyright 2026 linliangjun
 */

#include "elf_loader.h"
#include "ramfs.h"
#include "vmm.h"
#include "kernel.h"
#include "elf32.h"
#include "string.h"

i32 load_phdr(elf32_phdr_t *phdr, ramfs_file_t *file, u32 e_phoff)
{
    size_t read_count = ramfs_read(file, phdr, sizeof(elf32_phdr_t));
    return read_count < sizeof(elf32_ehdr_t) ? ELF_LOAD_ERROR_INVALID_FORMAT : 0;
}

i32 load_elf(const char *path, uintptr_t *entry)
{
    ramfs_file_t file = ramfs_open(path);
    if (file.start_addr == 0)
        return file.is_dir ? ELF_LOAD_ERROR_IS_DIRECTORY : ELF_LOAD_ERROR_FILE_NOT_FOUND;

    elf32_ehdr_t ehdr;
    size_t read_count = ramfs_read(&file, &ehdr, sizeof(elf32_ehdr_t));
    if (read_count < sizeof(elf32_ehdr_t))
        return ELF_LOAD_ERROR_INVALID_FORMAT;

    *entry = ehdr.e_entry;

    for (size_t i = 0; i < ehdr.e_phnum; i++)
    {
        elf32_phdr_t phdr;
        file.offset =  ehdr.e_phoff + i * ehdr.e_phentsize;
        read_count = ramfs_read(&file, &phdr, sizeof(elf32_phdr_t));
        if (read_count < sizeof(elf32_phdr_t))
            return ELF_LOAD_ERROR_INVALID_FORMAT;

        if(phdr.p_type != PT_LOAD)
            continue;

        file.offset = phdr.p_offset;
        ramfs_read(&file, (void *)phdr.p_vaddr, phdr.p_filesz);

        if (phdr.p_filesz < phdr.p_memsz)
            memset((void *)phdr.p_vaddr + phdr.p_filesz, 0, phdr.p_memsz - phdr.p_filesz);
    }
    return 0;
}

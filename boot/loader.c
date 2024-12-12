/**
 * 内核加载器
 *
 * Copyright 2024 linliangjun
 */

#include "loader.h"
#include "disk.h"
#include "elf32.h"
#include "string.h"

#define KERNEL_LBA 32

static unsigned char __disk_read(
    unsigned char driver_id, unsigned int lba, unsigned short offset)
{
    disk_read_req_t req = {.count = 1, .lba = lba, .segement = 0, .offset = offset};
    unsigned char code = disk_read(driver_id, &req);
    if (code)
        return code;
    req.offset += 512;
    req.lba++;
    code = disk_read(driver_id, &req);
    if (code)
        return code;
    return 0;
}

unsigned char load_kernel(unsigned char driver_id, unsigned int *kernel_entry)
{
    char buf[1024];

    // 读取 ELF 文件头
    unsigned char code = __disk_read(driver_id, KERNEL_LBA, (unsigned int)buf);
    if (code)
        return code;

    elf32_ehdr_t ehdr;
    memcpy(&ehdr, buf, sizeof(elf32_ehdr_t));
    *kernel_entry = ehdr.e_entry;

    // 遍历程序头
    for (unsigned short i = 0; i < ehdr.e_phnum; i++)
    {
        // 加载当前程序头
        unsigned int ph_offset = ehdr.e_phoff + i * ehdr.e_phentsize;
        code = __disk_read(driver_id, KERNEL_LBA + ph_offset / 512, (unsigned int)buf);
        if (code)
            return code;
        elf32_phdr_t phdr;
        memcpy(&phdr, buf + ph_offset % 512, sizeof(elf32_phdr_t));

        // 加载段
        if (phdr.p_type != PT_LOAD)
            continue;
        unsigned int memSize = phdr.p_memsz;
        unsigned int physAddr = phdr.p_paddr;
        if (!phdr.p_filesz)
        {
            memset((void *)physAddr, 0, memSize);
            continue;
        }
        unsigned int offset = phdr.p_offset;
        do
        {
            code = __disk_read(driver_id, KERNEL_LBA + offset / 512, (unsigned int)buf);
            if (code)
                return code;
            if (memSize >= 512)
            {
                memcpy((void *)physAddr, buf + offset % 512, 512);
                offset += 512;
                physAddr += 512;
                memSize -= 512;
            }
            else
            {
                memcpy((void *)physAddr, buf + offset % 512, memSize);
                memSize = 0;
            }
        } while (memSize);
    }
    return 0;
}

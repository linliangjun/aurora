/**
 * 内核加载器
 *
 * Copyright 2025 linliangjun
 */

#include "kernel_loader.h"
#include "disk.h"
#include "config.h"
#include "elf32.h"
#include "string.h"

#define min(a, b) (a) > (b) ? (b) : (a)

static unsigned char load_kernel_ehdr(unsigned char driver_id, elf32_ehdr_t *ehdr)
{
    char buf[512];
    unsigned char code = disk_read(driver_id, KERNEL_LBA, 1, buf);
    if (!code)
        memcpy(ehdr, buf, sizeof(elf32_ehdr_t));
    return code;
}

static unsigned char load_kernel_phdr(unsigned char driver_id,
                                      unsigned int offset, elf32_phdr_t *phdr)
{
    char buf[1024];
    unsigned char code = disk_read(driver_id, KERNEL_LBA + offset / 512, 2, buf);
    if (!code)
        memcpy(phdr, buf + offset % 512, sizeof(elf32_phdr_t));
    return code;
}

static unsigned char load_kernel_segment(unsigned char driver_id,
                                         const elf32_phdr_t *phdr,
                                         const ards_t *ards_vec,
                                         unsigned int ards_vec_size)
{
    // 非 PT_LOAD 类型的段，跳过
    if (phdr->p_type != PT_LOAD)
        return 0;

    // 判断段载入的内存是否可用
    _Bool available = 0;
    unsigned int segment_start = phdr->p_paddr, segment_end = segment_start + phdr->p_memsz;
    for (unsigned int i = 0; i < ards_vec_size; i++)
    {
        const ards_t *ards = ards_vec + i;
        if (ards->type != ARDS_TYPE_UNUSED)
            continue;
        unsigned int ards_start = ards->base_low, ards_end = ards_start + ards->len_low;
        if (segment_start >= ards_start && segment_start < ards_end && segment_end <= ards_end)
        {
            available = 1;
            break;
        }
    }
    if (!available)
        return 1;

    // count 表示已加载的字节数量，buf 表示缓冲区
    unsigned int count = 0;
    char *buf = (char *)phdr->p_paddr;
    char _buf[512]; // 临时缓冲区
    while (count < phdr->p_filesz)
    {
        unsigned int offset = phdr->p_offset + count, // 相对于 KERNEL_LBA 的字节偏移量
            remaining_count = phdr->p_filesz - count, // 剩余字节数
            lba = KERNEL_LBA + offset / 512,          // 当前扇区号
            byte_count;                               // 读取的字节数

        unsigned short sector_offset = offset % 512; // 扇区内字节偏移量
        unsigned char code;                          // 错误码

        // 扇区内偏移量不为 0
        if (sector_offset)
        {
            // 读取当前扇区到临时缓冲区，从扇区内偏移量开始，直到扇区结束，写入 buf
            // 若剩余字节数不能补齐，则长度按照剩余字节数，写入 buf
            code = disk_read(driver_id, lba, 1, _buf);
            if (code)
                return code;
            byte_count = min(512 - sector_offset, remaining_count);
            memcpy(buf, _buf + sector_offset, byte_count);
        }
        else
        {
            // 一次性读取剩余字节数中 512 的整倍数到 buf 中
            // 若剩余字节数不足 512，则先写入临时缓冲区，再写入 buf
            unsigned int sector_count = remaining_count / 512;
            if (sector_count > 0)
            {
                sector_count = min(sector_count, 255); // 最多一次性读取 255 个扇区
                code = disk_read(driver_id, lba, sector_count, buf);
                if (code)
                    return code;
                byte_count = sector_count * 512;
            }
            else
            {
                code = disk_read(driver_id, lba, 1, _buf);
                if (code)
                    return code;
                byte_count = remaining_count;
                memcpy(buf, _buf, byte_count);
            }
        }
        count += byte_count;
        buf += byte_count;
    }

    // 处理 bss
    if (phdr->p_filesz < phdr->p_memsz)
        memset(buf, 0, phdr->p_memsz - phdr->p_filesz);
    return 0;
}

unsigned char load_kernel(unsigned char driver_id,
                          unsigned int *kernel_entry,
                          const ards_t *ards_vec,
                          unsigned int ards_vec_size)
{
    // 加载文件头
    elf32_ehdr_t ehdr;
    unsigned char code = load_kernel_ehdr(driver_id, &ehdr);
    if (code)
        return code;
    *kernel_entry = ehdr.e_entry;

    // 遍历程序头
    for (unsigned short i = 0; i < ehdr.e_phnum; i++)
    {
        elf32_phdr_t phdr;
        code = load_kernel_phdr(driver_id, ehdr.e_phoff + i * ehdr.e_phentsize, &phdr);
        if (code)
            return code;

        // 加载段
        code = load_kernel_segment(driver_id, &phdr, ards_vec, ards_vec_size);
        if (code)
            return code;
    }
    return 0;
}

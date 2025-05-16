/**
 * 内核加载器
 *
 * Copyright 2025 linliangjun
 */

#include "boot/loader.h"
#include "boot/boot.h"
#include "disk.h"
#include "elf32.h"
#include "math.h"
#include "string.h"

static u8 load_kernel_ehdr(u8 drive_id, elf32_ehdr_t *ehdr)
{
    u8 buf[SECTOR_SIZE];
    u8 code = disk_read(drive_id, KERNEL_LBA, 1, buf);
    if (!code)
        memcpy(ehdr, buf, sizeof(elf32_ehdr_t));
    return code;
}

static u8 load_kernel_phdr(u8 drive_id, u32 offset, elf32_phdr_t *phdr)
{
    u8 buf[SECTOR_SIZE * 2];
    u8 code = disk_read(drive_id, KERNEL_LBA + offset / SECTOR_SIZE, 2, buf);
    if (!code)
        memcpy(phdr, buf + offset % SECTOR_SIZE, sizeof(elf32_phdr_t));
    return code;
}

static u8 load_kernel_segment(u8 drive_id, const elf32_phdr_t *phdr)
{
    // 非 PT_LOAD 类型的段，跳过
    if (phdr->p_type != PT_LOAD)
        return 0;

    // count 表示已加载的字节数量，buf 表示缓冲区
    u32 count = 0;
    u8 *buf = (u8 *)phdr->p_paddr;
    u8 _buf[SECTOR_SIZE]; // 临时缓冲区
    while (count < phdr->p_filesz)
    {
        u32 offset = phdr->p_offset + count,          // 相对于 KERNEL_LBA 的字节偏移量
            remaining_count = phdr->p_filesz - count, // 剩余字节数
            lba = KERNEL_LBA + offset / SECTOR_SIZE,  // 当前扇区号
            byte_count;                               // 读取的字节数

        u16 sector_offset = offset % SECTOR_SIZE; // 扇区内字节偏移量
        u8 code;                                  // 错误码

        // 扇区内偏移量不为 0
        if (sector_offset)
        {
            // 读取当前扇区到临时缓冲区，从扇区内偏移量开始，直到扇区结束，写入 buf
            // 若剩余字节数不能补齐，则长度按照剩余字节数，写入 buf
            code = disk_read(drive_id, lba, 1, _buf);
            if (code)
                return code;
            byte_count = MIN(SECTOR_SIZE - sector_offset, remaining_count);
            memcpy(buf, _buf + sector_offset, byte_count);
        }
        else
        {
            // 一次性读取剩余字节数中 SECTOR_SIZE 的整倍数到 buf 中
            // 若剩余字节数不足 SECTOR_SIZE，则先写入临时缓冲区，再写入 buf
            u32 sector_count = remaining_count / SECTOR_SIZE;
            if (sector_count > 0)
            {
                sector_count = MIN(sector_count, U8_MAX); // 最多一次性读取 U8_MAX 个扇区
                code = disk_read(drive_id, lba, sector_count, buf);
                if (code)
                    return code;
                byte_count = sector_count * SECTOR_SIZE;
            }
            else
            {
                code = disk_read(drive_id, lba, 1, _buf);
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

u8 load_kernel(u8 drive_id, u32 *kernel_entry)
{
    // 加载文件头
    elf32_ehdr_t ehdr;
    u8 code = load_kernel_ehdr(drive_id, &ehdr);
    if (code)
        return code;
    *kernel_entry = ehdr.e_entry;

    // 遍历程序头
    for (u16 i = 0; i < ehdr.e_phnum; i++)
    {
        elf32_phdr_t phdr;
        code = load_kernel_phdr(drive_id, ehdr.e_phoff + i * ehdr.e_phentsize, &phdr);
        if (code)
            return code;

        // 加载段
        code = load_kernel_segment(drive_id, &phdr);
        if (code)
            return code;
    }
    return 0;
}

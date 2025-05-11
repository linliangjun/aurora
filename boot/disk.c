/**
 * 磁盘驱动
 *
 * Copyright 2025 linliangjun
 */

#include "disk.h"
#include "boot/bios_call.h"
#include "math.h"
#include "string.h"

#define SECTOR_COUNT_PER_READ 4 // 每次读取扇区数量
#define SECTOR_SIZE 512         // 扇区大小（单位：字节）

/* 驱动器信息 */
typedef struct
{
    u8 drive_id;          // 驱动器号
    u8 sector_per_track;  // 每磁道扇区数
    u8 head_per_cylinder; // 每柱面磁头数
} drive_info_t;

static drive_info_t info;

/* 设置驱动器信息缓存 */
static u8 setup_drive_info(u8 drive_id)
{
    if (drive_id == info.drive_id && info.sector_per_track)
        return 0;
    u16 max_cylinder_sector;
    u8 max_head;
    u8 code = int_13h_08h(drive_id, &max_cylinder_sector, &max_head);
    if (!code)
    {
        info.drive_id = drive_id;
        info.sector_per_track = max_cylinder_sector & 0x3f;
        info.head_per_cylinder = max_head + 1;
    }
    return code;
}

static void lba_to_chs(u32 lba, u16 *cyliner, u8 *head, u8 *sector)
{
    u32 tracks = lba / info.sector_per_track;
    *cyliner = tracks / info.head_per_cylinder;
    *head = tracks % info.head_per_cylinder;
    *sector = lba % info.sector_per_track + 1;
}

static u8 _disk_read(u32 *lba, u8 *count, u8 **buf)
{
    // 若指定的缓冲区在 1 MiB 以上，则用栈作为临时缓冲区
    u8 *_buf = *buf;
    bool require_memcpy = false;
    if ((uintptr_t)_buf >= 0x100000)
    {
        u8 __buf[SECTOR_SIZE * SECTOR_COUNT_PER_READ];
        _buf = __buf;
        require_memcpy = true;
    }

    // i 为已读取的扇区数量，total 为总共需要读取的扇区数量
    u8 i = 0, total = MIN(SECTOR_COUNT_PER_READ, *count);
    while (i < total)
    {
        // LBA 转 CHS
        u8 head, sector;
        u16 cyliner;
        lba_to_chs(*lba, &cyliner, &head, &sector);

        // 计算本次需要读取的扇区数量（解决了跨磁道读取问题）
        u8 _count = MIN(total - i, info.sector_per_track - sector + 1);

        // 读取磁盘扇区
        u8 code = int_13h_02h(info.drive_id, CYLINDER_SECTOR(cyliner, sector),
                              head, _count, (uintptr_t)_buf >> 4, (uintptr_t)_buf & 0xf);
        if (code)
            return code;

        // 准备下次循环
        i += _count;
        *lba += _count;

        // 复制数据（如果需要）
        u16 bytes = _count * SECTOR_SIZE;
        if (require_memcpy)
            memcpy(*buf, _buf, bytes);
        else
            _buf += bytes;
        *buf += bytes;
    }
    *count -= total;
    return 0;
}

u8 disk_read(u8 drive_id, u32 lba, u8 count, void *buf)
{
    // 设置驱动器信息缓存，避免频繁进行 BIOS 调用
    u8 code = setup_drive_info(drive_id);
    if (code)
        return code;

    // 读取扇区数据，直到 count 为 0
    u8 *_buf = (u8 *)buf;
    while (count)
    {
        code = _disk_read(&lba, &count, &_buf);
        if (code)
            return code;
    }
    return 0;
}

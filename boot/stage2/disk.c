/**
 * 磁盘驱动
 *
 * Copyright 2025 linliangjun
 */

#include "bios_call.h"
#include "string.h"

/* 驱动器参数 */
typedef struct
{
    unsigned char driver_id;         // 驱动器号
    unsigned char sector_per_track;  // 每磁道扇区数
    unsigned char head_per_cylinder; // 每柱面磁头数
} driver_params_t;

#define INT13H_CYLINDER_SECTOR(cylinder, sector) \
    ((cylinder) << 8) | (((cylinder) >> 2) & 0xc0) | (sector)

#define min(a, b) (a) > (b) ? (b) : (a)

#define SECTOR_COUNT_PER_READ 8 // 每次读取扇区数量

static driver_params_t driver_params_cache;

/* 设置驱动器参数缓存 */
static unsigned char setup_driver_params_cache(unsigned char driver_id)
{
    if (driver_id == driver_params_cache.driver_id && driver_params_cache.sector_per_track)
        return 0;
    unsigned short max_cylinder_sector;
    unsigned char max_head;
    unsigned char code = int_13h_08h(driver_id, &max_cylinder_sector, &max_head);
    if (!code)
    {
        driver_params_cache.driver_id = driver_id;
        driver_params_cache.sector_per_track = max_cylinder_sector & 0x3f;
        driver_params_cache.head_per_cylinder = max_head + 1;
    }
    return code;
}

static void lba_to_chs(unsigned int lba,
                       unsigned short *cyliner,
                       unsigned char *head,
                       unsigned char *sector)
{
    unsigned int tracks = lba / driver_params_cache.sector_per_track;
    *cyliner = tracks / driver_params_cache.head_per_cylinder;
    *head = tracks % driver_params_cache.head_per_cylinder;
    *sector = lba % driver_params_cache.sector_per_track + 1;
}

static unsigned char _disk_read(unsigned char driver_id,
                                unsigned int *lba,
                                unsigned char *count,
                                char *buf)
{
    // 设置缓冲区
    char *_buf = buf;
    _Bool require_memcpy = 0;
    if ((unsigned int)buf >= 0x100000)
    {
        char __buf[512 * SECTOR_COUNT_PER_READ];
        _buf = (char *)__buf;
        require_memcpy = 1;
    }

    // i 为已读取的扇区数量，total 为总共需要读取的扇区数量
    unsigned char i = 0, total = min(SECTOR_COUNT_PER_READ, *count);
    while (i < total)
    {
        // LBA 转 CHS
        unsigned char head, sector;
        unsigned short cyliner;
        lba_to_chs(*lba, &cyliner, &head, &sector);

        // 计算本次需要读取的扇区数量（解决跨磁道读取问题）
        unsigned char remaining_count = total - i; // 剩余未读取的扇区数量
        unsigned char _count = sector - 1 + remaining_count > driver_params_cache.sector_per_track
                                   ? driver_params_cache.sector_per_track - sector + 1
                                   : remaining_count;

        // 读取磁盘扇区
        unsigned char code = int_13h_02h(driver_id, INT13H_CYLINDER_SECTOR(cyliner, sector),
                                         head, _count, (unsigned int)_buf >> 4,
                                         (unsigned int)_buf & 0xf);
        if (code)
            return code;

        // 复制数据（如果需要）
        if (require_memcpy)
            memcpy(buf + i * 512, _buf, _count * 512);

        // 准备下次循环
        i += _count;
        *lba += _count;
    }
    *count -= total;
    return 0;
}

/**
 * 读取磁盘扇区
 *
 * 注意：由于使用 CHS 模式定位扇区，因此 LBA 理论上最大为 16515071
 *
 * @param driver_id 驱动器号
 * @param lba 逻辑扇区号
 * @param count 读取扇区数量
 * @param buf 缓冲区
 *
 * @return 错误码，0 表示正常
 */
unsigned char disk_read(unsigned char driver_id,
                        unsigned int lba,
                        unsigned char count,
                        char *buf)
{
    // 设置驱动器参数缓存，避免频繁进行 BIOS 调用
    unsigned char code = setup_driver_params_cache(driver_id);
    if (code)
        return code;

    // 读取扇区数据，直到 count 为 0
    while (count)
    {
        unsigned char old_count = count;
        code = _disk_read(driver_id, &lba, &count, buf);
        if (code)
            return code;
        buf += (old_count - count) * 512;
    }
    return 0;
}

/**
 * 磁盘驱动
 *
 * Copyright 2024 linliangjun
 */

#include "disk.h"
#include "bioscall.h"

unsigned char disk_read(unsigned char driver_id, const disk_read_req_t *req)
{
    driver_params_t params;
    unsigned char code = int_13h_08h(driver_id, &params);
    if (code)
        return code;

    // LBA 转 CHS
    unsigned char sector_per_track = params.max_cylinder_sector & 0x3f;
    unsigned char head_per_cylinder = params.max_head + 1;

    unsigned int tracks = req->lba / sector_per_track;
    unsigned char head = tracks % head_per_cylinder;
    unsigned short cyliner = tracks / head_per_cylinder;
    unsigned char sector = req->lba % sector_per_track + 1;

    driver_read_req_t driver_req =
        {INT13H_CYLINDER_SECTOR(cyliner, sector), head, req->count, req->segement, req->offset};
    return int_13h_02h(driver_id, &driver_req);
}

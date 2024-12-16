/**
 * 内存
 *
 * Copyright 2024 linliangjun
 */

#ifndef __AURORA_MEM_H
#define __AURORA_MEM_H

#include "aurora/types.h"

typedef struct
{
    u32 base_low;
    u32 base_high;
    u32 len_low;
    u32 len_high;
    u32 type;
} ards_t;

typedef struct
{
    bool p : 1; // 是否存在。true 表示存在；false 表示不存在
    u8 rw : 1;  // 读写位。1 表示可读可写；0 表示可读不可写
    u8 us : 1;  // 权限。1 表示任何级别的代码都可访问；0 表示只允许特权级为 [0, 2] 的代码可访问
    u8 pwt : 1;
    u8 pcd : 1;
    bool access : 1;
    bool diry : 1;
    u8 pat : 1;
    bool global : 1;
    u8 available : 3; // 保留
    u32 index : 20;   // 索引
} page_entry_t;

#define ARDS_TYPE_AVL 1
#define ARDS_TYPE_RESV 2

void mem_init(u8 _ards_vec_size, const ards_t *_ards_vec);

#endif

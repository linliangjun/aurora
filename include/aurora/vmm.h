/**
 * 虚拟内存管理器
 *
 * Copyright 2025 linliangjun
 */

#ifndef __AURORA_VMM_H
#define __AURORA_VMM_H

#include "aurora/mem.h"

/* 页表/页目录项 */
typedef struct __attribute__((packed))
{
    bool p : 1;       // 是否存在。true 表示存在；false 表示不存在
    bool rw : 1;      // 读写位。true 表示可读可写；0 表示可读不可写
    u8 us : 1;        // 权限。1 表示任何级别的代码都可访问； 0 表示只允许特权级为 [0, 2] 的代码可访问
    u8 pwt : 1;       // 页级通写位
    u8 pcd : 1;       // 页级缓存禁止位
    bool access : 1;  // 访问位
    bool diry : 1;    // 脏页位
    u8 pat : 1;       // 页属性表位（页表项），页目录项置 0
    bool global : 1;  // 全局位
    u8 available : 3; // 保留
    u32 index : 20;   // 索引
} page_entry_dir_t;

void vmm_init(void);

#endif

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

#define ARDS_TYPE_AVL 1
#define ARDS_TYPE_RESV 2

void mem_init(void);

#endif

/**
 * 物理内存管理器
 *
 * Copyright 2025 linliangjun
 */

#ifndef __AURORA_PMM_H
#define __AURORA_PMM_H

#include "aurora/boot_params.h"

void pmm_init(const boot_params_t *boot_params);

#endif

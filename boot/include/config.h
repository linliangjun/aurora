/**
 * 引导配置
 *
 * Copyright 2025 linliangjun
 */

#ifndef __BOOT_CONFIG_H
#define __BOOT_CONFIG_H

#define STAGE2_CYLINDER 0       // 第二阶段引导起始柱面号
#define STAGE2_HEAD 0           // 第二阶段引导起始磁头号
#define STAGE2_SECTOR 2         // 第二阶段引导起始扇区号
#define STAGE2_SECTOR_COUNT 15  // 第二阶段引导扇区数量
#define STAGE2_ADDR 0x7e00      // 第二阶段引导地址

#define KERNEL_LBA 32           // 内核逻辑扇区号

#define REAL_STACK_INIT 0x7c00  // 16 位实地址模式初始栈
#define PROT_STACK_INIT 0x90000 // 32 位保护模式初始栈

#endif

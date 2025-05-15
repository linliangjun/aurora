/**
 * 中断描述符表
 *
 * Copyright 2025 linliangjun
 */

#ifndef __IDT_H
#define __IDT_H

#include "seg.h"

/* 描述符类型 */
#define GATE_TYPE_TASK 0b0101         // 任务门
#define GATE_TYPE_INTERRUPT_16 0b0110 // 16 位中断门
#define GATE_TYPE_TRAP_16 0b0111      // 16 位陷阱门
#define GATE_TYPE_INTERRUPT_32 0b1110 // 32 位中断门
#define GATE_TYPE_TRAP_32 0b1111      // 32 位陷阱门

/* 门描述符 */
typedef struct
{
    u8 value[8];
} gate_desc_t;

/* 中断门描述符 */
typedef struct __attribute__((packed))
{
    u16 offset_low;           // 段内偏移 0 ~ 15 位
    seg_desc_sele_t seg_sele; // 段选择子
    u8 reserved : 5;          // 保留
    u8 zero_1 : 3;            // 置 0
    u8 type : 4;              // 类型
    u8 zero_2 : 1;            // 置 0
    u8 dpl : 2;               // 权限。范围为 [0, 3]，0 为最高级，3 为最低级
    bool p : 1;               // 是否存在。true 表示存在；false 表示不存在
    u16 offset_high;          // 段内偏移 16 ~ 31 位
} interrupt_gate_desc_t;

/* 中断上下文 */
typedef struct
{
    u32 eip;
    u16 cs;
    u16 reserved;
    u32 eflags;
} interrupt_frame_t;

/**
 * 中断门描述符
 *
 * @param _seg_sele 段选择子
 * @param offset 段内偏移
 * @param _type 类型
 * @param _dpl 权限
 * @param _p 是否存在。true 表示存在；false 表示不存在
 */
#define INTERRUPT_GATE_DESC(_seg_sele, offset, _type, _dpl, _p) \
    (interrupt_gate_desc_t){                                    \
        .offset_low = (offset) & 0xffff,                        \
        .seg_sele = _seg_sele,                                  \
        .reserved = 0,                                          \
        .zero_1 = 0,                                            \
        .type = _type,                                          \
        .zero_2 = 0,                                            \
        .dpl = _dpl,                                            \
        .p = _p,                                                \
        .offset_high = (offset) >> 16}

/**
 * 初始化 IDT
 */
void idt_init(void);

/**
 * 设置门描述符
 */
void idt_set_desc(u8 vector, const gate_desc_t *desc);

/**
 * 设置中断门描述符
 */
void set_interrupt_gate_desc(u8 vector, const interrupt_gate_desc_t *desc);

#endif // __IDT_H

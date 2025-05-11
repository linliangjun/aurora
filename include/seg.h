/**
 * 保护模式段
 *
 * Copyright 2025 linliangjun
 */

#ifndef __SEG_H
#define __SEG_H

/* 描述符类型 */
#define SEG_DESC_TYPE_WRITEABLE_DATA 0b0010 // 可写数据段
#define SEG_DESC_TYPE_CODE 0b1000           // 代码段

/**
 * 描述符访问类型
 *
 * @param p 是否存在。1 表示存在；0 表示不存在
 * @param dpl 权限。范围为 [0, 4)，0 为最高级，3 为最低级
 * @param s 是否为系统段。0 表示为系统段（任务段）；1 表示为非系统段（代码段、数据段）
 * @param type 类型
 */
#define SEG_DESC_ACCESS(p, dpl, s, type) \
    ((((p) & 1) << 7) | (((dpl) & 3) << 5) | (((s) & 1) << 4) | ((type) & 0xf))

/**
 * 描述符标记
 *
 * @param g 粒度。1 表示 4 KiB；0 表示 1 字节
 * @param db 操作数大小。1 表示 32 位；0 表示 16 位
 * @param l 是否为长模式。1 表示 64 位代码段；0 表示非 64 位代码段
 */
#define SEG_DESC_FLAGS(g, db, l) \
    ((((g) & 1) << 3) | (((db) & 1) << 2) | (((l) & 1) << 1))

/**
 * 描述符
 *
 * @param base 基地址。32 位
 * @param lim 界限。20 位
 * @param access 访问类型。使用宏 SEG_DESC_ACCESS 生成
 * @param flags 标记。使用宏 SEG_DESC_FLAGS 生成
 */
#define SEG_DESC(base, lim, access, flags)    \
    .word((lim) & 0xffff), ((base) & 0xffff); \
    .byte(((base) >> 16) & 0xff), (access), (((flags) << 4) | ((lim) >> 16)), ((base) >> 24)

/* 空描述符 */
#define SEG_DESC_NULL .quad 0

/**
 * 描述符选择子
 *
 * @param index 索引。13 位
 * @param ti 表指示符。0 表示在 GDT 中；1 表示在 LDT 中
 * @param rpl 请求特权级。范围为 [0, 4)，0 为最高级，3 为最低级
 */
#define SEG_DESC_SELE(index, ti, rpl) (((index) << 3) | ((ti) << 2) | (rpl))

#endif // __SEG_H

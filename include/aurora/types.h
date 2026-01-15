/**
 * 基本数据类型
 *
 * Copyright 2025 linliangjun
 */

#ifndef __AURORA_TYPES_H
#define __AURORA_TYPES_H

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long int u64;

typedef signed char i8;
typedef signed short i16;
typedef signed int i32;
typedef signed long long int i64;

#define I32_MAX 0x7fffffff
#define U8_MAX 0xff

typedef u32 size_t;

typedef u32 uintptr_t;

#ifndef __cplusplus
typedef _Bool bool;
#endif

#define true 1
#define false 0

#endif // __AURORA_TYPES_H

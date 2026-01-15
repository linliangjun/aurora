/**
 * 数学工具库
 *
 * Copyright 2025 linliangjun
 */

#ifndef __AURORA_MATH_H
#define __AURORA_MATH_H

#define MIN(a, b) ((a) > (b) ? (b) : (a))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

#define DIV_ROUND_UP(n, d) ((n) + (d) - 1) / (d)

#define RANGE_CONTAINS(base, len, start, offset) \
    ((start) < (base) ? false : (start) - (base) + (offset) <= (len))

#endif // __AURORA_MATH_H

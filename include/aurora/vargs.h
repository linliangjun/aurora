/**
 * 可变参数
 *
 * Copyright 2025 linliangjun
 */

#ifndef __AURORA_VARGS_H
#define __AURORA_VARGS_H

#include "aurora/types.h"

typedef char *va_list;

#define _INTSIZEOF(n) ((sizeof(n) + sizeof(size_t) - 1) & ~(sizeof(size_t) - 1))

#define va_start(ap, v) (ap = (va_list) & v + _INTSIZEOF(v))
#define va_arg(ap, t) (*(t *)((ap += _INTSIZEOF(t)) - _INTSIZEOF(t)))
#define va_end(ap) (ap = (va_list)0)

#endif // __AURORA_VARGS_H

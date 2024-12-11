/**
 * 可变参数格式化打印
 *
 * Copyright 2024 linliangjun
 */

#ifndef __VSPRINTF_H
#define __VSPRINTF_H

#include <stdarg.h>

unsigned int vsprintf(char *buf, const char *fmt, va_list args);

#endif

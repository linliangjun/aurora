/**
 * 标准库
 *
 * Copyright 2025 linliangjun
 */

#ifndef __STDLIB_H
#define __STDLIB_H

#define abort() \
    __abort();  \
    while (1)

void __abort(void);

#endif

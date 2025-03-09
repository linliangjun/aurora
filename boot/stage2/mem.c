/**
 * 内存
 *
 * Copyright 2025 linliangjun
 */

#include "mem.h"
#include "bios_call.h"
#include "string.h"

unsigned char detect_mem(ards_t *ards_vec, unsigned int *size)
{
    ards_t ards;
    unsigned int next = 0, _size = 0;
    unsigned char code;
    do
    {
        code = int_15h_e820h(&next, (unsigned int)&ards >> 4, (unsigned int)&ards & 0xf);
        memcpy(ards_vec + _size, &ards, sizeof(ards_t));
        _size++;
    } while (!code && next);
    *size = _size;
    return code;
}

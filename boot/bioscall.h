/**
 * BIOS 中断调用
 *
 * Copyright 2024 linliangjun
 */

#ifndef __BIOSCALL_H
#define __BIOSCALL_H

/**
 * 以 TTY 模式打印字符
 *
 * @param c 字符
 */
void int_10h_0eh(char c);

#endif

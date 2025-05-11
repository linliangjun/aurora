/**
 * BIOS 中断调用
 *
 * Copyright 2025 linliangjun
 */

#ifndef __BOOT_BIOS_CALL_H
#define __BOOT_BIOS_CALL_H

/**
 * 以 TTY 模式打印字符
 *
 * @param c 字符
 */
void int_10h_0eh(char c);

#endif // __BOOT_BIOS_CALL_H

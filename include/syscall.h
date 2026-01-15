/**
 * 系统调用
 *
 * Copyright 2026 linliangjun
 */

#ifndef __SYSCALL_H
#define __SYSCALL_H

#define SYS_WRITE 0x01

/**
 * 初始化系统调用
 */
void syscall_init(void);

/**
 * 系统调用：写字符串到终端
 *
 * @param str 要写入的字符串
 */
void sys_write(const char *str);

#endif

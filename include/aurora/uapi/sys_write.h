/**
 * 系统调用：写字符串到终端
 *
 * Copyright 2026 linliangjun
 */

#ifndef __AURORA_UAPI_SYS_WRITE_H
#define __AURORA_UAPI_SYS_WRITE_H

/**
 * 系统调用：写字符串到终端
 *
 * @param str 要写入的字符串
 */
void sys_write(const char *str);

#endif

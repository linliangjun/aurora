/**
 * 内核打印
 *
 * Copyright 2024 linliangjun
 */

#ifndef __AURORA_PRINTK_H
#define __AURORA_PRINTK_H

#define KERN_EMERG "<0>"   /* 紧急事件消息，系统崩溃之前提示，表示系统不可用 */
#define KERN_ALERT "<1>"   /* 报告消息，表示必须立即采取措施 */
#define KERN_CRIT "<2>"    /* 临界条件，通常涉及严重的硬件或软件操作失败 */
#define KERN_ERR "<3>"     /* 错误条件，驱动程序常用KERN_ERR来报告硬件的错误 */
#define KERN_WARNING "<4>" /* 警告条件，对可能出现问题的情况进行警告 */
#define KERN_NOTICE "<5>"  /* 正常但又重要的条件，用于提醒。常用于与安全相关的消息 */
#define KERN_INFO "<6>"    /* 提示信息，如驱动程序启动时，打印硬件信息 */
#define KERN_DEBUG "<7>"   /* 调试级别的消息 */

#ifndef pr_fmt

#define pr_fmt(fmt) "%s#%d - " fmt, __FILE__, __LINE__

#endif

#define pr_emerg(fmt, ...) \
    printk(KERN_EMERG pr_fmt(fmt), ##__VA_ARGS__)
#define pr_alert(fmt, ...) \
    printk(KERN_ALERT pr_fmt(fmt), ##__VA_ARGS__)
#define pr_crit(fmt, ...) \
    printk(KERN_CRIT pr_fmt(fmt), ##__VA_ARGS__)
#define pr_err(fmt, ...) \
    printk(KERN_ERR pr_fmt(fmt), ##__VA_ARGS__)
#define pr_warn(fmt, ...) \
    printk(KERN_WARNING pr_fmt(fmt), ##__VA_ARGS__)
#define pr_notice(fmt, ...) \
    printk(KERN_NOTICE pr_fmt(fmt), ##__VA_ARGS__)
#define pr_info(fmt, ...) \
    printk(KERN_INFO pr_fmt(fmt), ##__VA_ARGS__)

#ifdef DEBUG

#define pr_debug(fmt, ...) \
    printk(KERN_DEBUG pr_fmt(fmt), ##__VA_ARGS__)

#else

#define pr_debug(fmt, ...)

#endif

void printk(const char *fmt, ...);

#endif

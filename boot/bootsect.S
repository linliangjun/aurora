/**
 * 通过 BIOS Legacy 方式加载的引导代码 —— MBR
 *
 * MBR 位于磁盘的第一个扇区，且以 0x55 和 0xaa 结尾。
 * BIOS 会将该扇区加载到 0x7c00，并转移控制权。
 * 转移后，CPU 处于 16 位实地址模式，cs 和 ip 分别为 0 和 0x7c00。
 *
 * Copyright 2025 linliangjun
 */

#define STACK_TOP 0x7c00
#define TTY_WRITE(addr) cld; mov $addr, %si; call tty_write
#define MBR_MAGIC .org 0x1fe; .byte 0x55, 0xaa

    .globl      _start

    .text
    .code16
_start:
    // 先关闭中断，在设置完段寄存器后再开启
    cli

    // 有一些 BIOS 会有 bug，cs 不为 0，通过长跳转可以让 cs 为 0
    jmp         $0, $1f
1:
    // 让 ds、es 和 ss 都为 0，并设置栈顶
    xor         %ax, %ax
    mov         %ax, %ds
    mov         %ax, %es
    mov         %ax, %ss
    mov         $STACK_TOP, %sp

    // 重新开启中断
    sti

    // 打印消息
    TTY_WRITE(msg)

    // 无限循环
    jmp         .


/**
 * TTY 打印字符串
 *
 * @param ds:si 字符串首地址
 */
1:
    mov         $0xe, %ah
    int         $0x10
tty_write:
    lodsb
    cmp         $0, %al
    jne         1b
    ret

msg:
    .string     "Aurora\r\n"

    // MBR 魔数
    MBR_MAGIC

/**
 * 通过 BIOS Legacy 方式加载的引导代码（MBR）
 *
 * MBR 位于磁盘的第一个扇区，且以 0x55 和 0xaa 结尾。
 * BIOS 会将该扇区加载到 0x7c00，并转移控制权。
 * 转移后，CPU 处于 16 位实地址模式，cs 和 ip 分别为 0 和 0x7c00。
 *
 * 该代码会以固定扇区块的方式加载第二阶段引导代码到 0x7e00，
 * 并转移控制权。此外，栈中保存着驱动器号（2 个字节）。
 *
 * Copyright 2025 linliangjun
 */

#include "boot/boot.h"

#define STACK_TOP 0x7c00
#define MAX_ATTEMPTS 3

#define CYLINDER_SECTOR(c, s) (((c) << 8) | (((c) >> 2) & 0xc0) | (s))
#define MBR_MAGIC .org 0x1fe; .byte 0x55, 0xaa

    .globl      _start, tty_write

    .section    .text.startup, "ax"
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

    // 保存驱动器号到栈中
    push        %dx

    /* 开始加载第二阶段引导代码，具有失败重试机制 */
    // 保存重试次数（0）到栈中
    push        $0
1:
    mov         $(0x200 + STAGE2_SECTOR_COUNT), %ax
    mov         $STAGE2_ADDR, %bx
    mov         $CYLINDER_SECTOR(STAGE2_CYLINDER, STAGE2_SECTOR), %cx
    mov         $STAGE2_HEAD, %dh
    mov         -2(%bp), %dl
    int         $0x13

    // 调用出错（CF = 1），进入错误处理；否则将控制权转移给第二阶段引导代码
    jc          2f
    add         $2, %sp
    jmp         stage2_entry

2:
    // 重试次数 + 1
    incb        -4(%bp)

    // 判断是否到最大重试次数
    // 若达到最大重试次数，则打印错误信息并停止；否则重置驱动器，并重试
    cmpb        $MAX_ATTEMPTS, -4(%bp)
    jae         3f
    xor         %ah, %ah
    int         $0x13
    jmp         1b

3:
    // 打印错误消息
    TTY_WRITE(error_msg)

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

error_msg:
    .string     "Failed to load the second stage.\r\n"

    // MBR 魔数
    MBR_MAGIC

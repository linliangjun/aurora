/**
 * 系统调用中断处理例程
 *
 * Copyright 2026 linliangjun
 */

#include "aurora/kernel/kernel.h"

    .globl syscall_entry

    .text
    .code32
syscall_entry:
    pusha
    push        %ds
    push        %es
    push        %fs
    push        %gs
    mov         $DATA_SEG_SELE, %ax      # 内核数据段选择子
    mov         %ax, %ds
    mov         %ax, %es
    mov         %ax, %fs
    mov         %ax, %gs
    push        %esp                     # regs_t *reg
    call        syscall_dispatch
    add         $4, %esp
    pop         %gs
    pop         %fs
    pop         %es
    pop         %ds
    popa
    iret

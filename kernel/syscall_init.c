/**
 * 系统调用
 *
 * Copyright 2026 linliangjun
 */

#include "aurora/kernel/syscall_init.h"
#include "aurora/kernel/idt.h"
#include "aurora/kernel/kernel.h"
#include "aurora/kernel/printk.h"
#include "aurora/kernel/tty.h"

extern void syscall_entry(void);

typedef struct {
    u32 gs, fs, es, ds;
    u32 edi, esi, ebp, esp_dummy, ebx, edx, ecx, eax;
} regs_t;

void syscall_dispatch(regs_t *regs)
{
    switch (regs->eax)
    {
    case 1:
        tty_write((char *) regs->ecx);
        regs->eax = 0;
    default:
        regs->eax = -1;
    }
}

void syscall_init(void)
{
    set_interrupt_gate_desc(0x80, &INTERRUPT_GATE_DESC(CODE_SEG_SELE, (uintptr_t)syscall_entry, GATE_TYPE_INTERRUPT_32, 3, true));
    PR_INFO("Syscall Initialized\n");
}

/**
 * 中断描述符表
 *
 * Copyright 2025 linliangjun
 */

#include "idt.h"
#include "assert.h"
#include "kernel.h"
#include "string.h"

#define EXCEPTION_HANDLER(vector, name)                                                                                        \
    __attribute__((interrupt)) static void exception_handler_##vector(const interrupt_frame_t *frame)                          \
    {                                                                                                                          \
        PR_EMERG("[" #vector "] " name ". cs: %#06x, eip: %#010x, eflags: %#010x\n", frame->cs, frame->eip, frame->eflags); \
        while (true)                                                                                                           \
            ;                                                                                                                  \
    }

#define EXCEPTION_HANDLER_WITH_CODE(vector, name)                                                               \
    __attribute__((interrupt)) static void exception_handler_##vector(const interrupt_frame_t *frame, u32 code) \
    {                                                                                                           \
        PR_EMERG("[" #vector "] " name ". cs: %#06x, eip: %#010x, eflags: %#010x, error code: %#010x\n",    \
                 frame->cs, frame->eip, frame->eflags, code);                                                   \
        while (true)                                                                                            \
            ;                                                                                                   \
    }

EXCEPTION_HANDLER(0x00, "Divide Error")
EXCEPTION_HANDLER_WITH_CODE(0x08, "Double Fault")
EXCEPTION_HANDLER_WITH_CODE(0x0d, "General Protection")

static void set_interrupt_gate_desc(u8 vector, const interrupt_gate_desc_t *desc)
{
    idt_set_desc(vector, (gate_desc_t *)desc);
    PR_DEBUG("Set IDT[%d], seg_sele: {rpl: %d, ti: %d, index: %d}, offset: %#010x, type: %04b, dpl: %d, p: %d\n",
             vector, desc->seg_sele.rpl, desc->seg_sele.ti, desc->seg_sele.index,
             (desc->offset_high << 16) | desc->offset_low, desc->type, desc->dpl, desc->p);
}

#define SET_EXCEPTION_HANDLER(vector) \
    set_interrupt_gate_desc(vector, &INTERRUPT_GATE_DESC(CODE_SEG_SELE, (uintptr_t)exception_handler_##vector, GATE_TYPE_INTERRUPT_32, 0, true))

typedef struct __attribute__((packed))
{
    u16 lim;
    uintptr_t base;
} idt_desc_t;

static gate_desc_t idt[IDT_ENTRY_COUNT] __attribute__((aligned(8)));

void idt_init(void)
{
    SET_EXCEPTION_HANDLER(0x00);
    SET_EXCEPTION_HANDLER(0x08);
    SET_EXCEPTION_HANDLER(0x0d);
    idt_desc_t idt_desc = {.lim = sizeof(idt) - 1, .base = (uintptr_t)idt};
    __asm__ __volatile__("lidt %0" : : "m"(idt_desc));
    PR_INFO("Load IDT, base: %#x, limit: %#x\n", idt_desc.base, idt_desc.lim);
}

void idt_set_desc(u8 vector, const gate_desc_t *desc)
{
    ASSERT(vector < IDT_ENTRY_COUNT, "Vector out of bound!");
    memcpy(idt + vector, desc, sizeof(gate_desc_t));
}

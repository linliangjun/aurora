/**
 * 任务管理器
 *
 * Copyright 2025 linliangjun
 */

#include "task_manager.h"
#include "gdt.h"
#include "kernel.h"
#include "heap.h"
#include "string.h"
#include "printk.h"

/**
 * TSS 结构体
 */
typedef struct __attribute__((packed))
{
    u32 pre_task_link; // 上一个任务的链表指针，高 16 位保留
    u32 esp0;
    seg_desc_sele_t ss0;
    u16 ss0_resv;
    u32 esp1;
    seg_desc_sele_t ss1;
    u16 ss1_resv;
    u32 esp2;
    seg_desc_sele_t ss2;
    u16 ss2_resv;
    u32 cr3;
    u32 eip;
    u32 eflags;
    u32 eax;
    u32 ecx;
    u32 edx;
    u32 ebx;
    u32 esp;
    u32 ebp;
    u32 esi;
    u32 edi;
    seg_desc_sele_t es; // 高 16 位保留
    u16 es_resv;
    seg_desc_sele_t cs; // 高 16 位保留
    u16 cs_resv;
    seg_desc_sele_t ss; // 高 16 位保留
    u16 ss_resv;
    seg_desc_sele_t ds; // 高 16 位保留
    u16 ds_resv;
    seg_desc_sele_t fs; // 高 16 位保留
    u16 fs_resv;
    seg_desc_sele_t gs; // 高 16 位保留
    u16 gs_resv;
    u32 ldt_selector; // 高 16 位保留
    u32 io_bitmap;    // 低 16 位保留
    u32 ssp;
} tss_t;

/**
 * 任务结构体
 */
typedef struct _task
{
    size_t id;          // 任务 ID
    u16 gdt_index;      // GDT 索引
    tss_t tss;          // TSS
    struct _task *next; // 下一个任务的指针
} task_t;

static task_t *kernel_task;
static task_t *current_task;

static size_t task_count = 0;

static u32 get_cr3(void)
{
    u32 cr3;
    __asm__ volatile("mov %%cr3, %0" : "=r"(cr3));
    return cr3;
}

void task_manager_init(void)
{
    // 创建内核 TSS
    kernel_task = heap_kmalloc(sizeof(task_t));
    memset(kernel_task, 0, sizeof(task_t));

    kernel_task->id = task_count++;
    kernel_task->next = 0;
    tss_t *tss = &kernel_task->tss;
    tss->cr3 = get_cr3();

    // 设置 TSS 描述符
    kernel_task->gdt_index = gdt_set_desc_auto(&SEG_DESC((uintptr_t)tss, sizeof(tss_t) - 1, SEG_DESC_ACCESS(1, 0, 0, SEG_DESC_TYPE_TASK), SEG_DESC_FLAGS(0, 1, 0)));
    __asm__ __volatile__("ltr %0" : : "r"(kernel_task->gdt_index << 3)); // TI=0, RPL=0

    current_task = kernel_task;

    PR_INFO("Task Manager Initialized\n");
}

size_t task_spawn(uintptr_t entry_point)
{
    task_t *task = heap_kmalloc(sizeof(task_t));
    memset(task, 0, sizeof(task_t));

    task->id = task_count++;
    task->next = current_task->next;
    current_task->next = task;
    tss_t *tss = &task->tss;
    tss->cr3 = get_cr3();

    // 分配栈（4 KiB）
    tss->esp = tss->esp0 = (uintptr_t)heap_kmalloc(4096) + 4096;
    tss->ss = tss->ss0 = DATA_SEG_SELE;

    tss->cs = CODE_SEG_SELE;
    tss->eip = entry_point;
    tss->ds = DATA_SEG_SELE;
    tss->es = DATA_SEG_SELE;
    tss->fs = DATA_SEG_SELE;
    tss->gs = DATA_SEG_SELE;
    tss->eflags = 0x202;

    // 设置 GDT 描述符
    task->gdt_index = gdt_set_desc_auto(&SEG_DESC((uintptr_t)tss, sizeof(tss_t) - 1, SEG_DESC_ACCESS(1, 0, 0, SEG_DESC_TYPE_TASK), SEG_DESC_FLAGS(0, 1, 0)));
    return task->id;
}

// 执行远跳转
static void far_jump(u16 cs, u32 eip)
{

    // 定义远跳转指针结构（32位模式）
    struct far_ptr
    {
        u32 eip;
        u16 cs;
    } __attribute__((packed));

    struct far_ptr target = {.eip = eip, .cs = cs};

    __asm__ volatile(
        "ljmp *%0" // 远跳转指令
        :
        : "m"(target) // 输入：内存操作数（far_ptr结构）
        : "memory"    // 破坏描述：内存被修改
    );
}

void task_manager_schedule(void)
{
    task_t *next_task = current_task->next;
    if (next_task == 0)
        next_task = kernel_task;
    if (current_task == next_task)
        return;
    u16 tss_selector = (next_task->gdt_index << 3); // TI=0, RPL=0
    current_task = next_task;
    far_jump(tss_selector, 0);
}

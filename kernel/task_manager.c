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
#include "bitmap.h"
#include "math.h"
#include "panic.h"

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
typedef struct
{
    u16 gdt_index; // GDT 索引
    tss_t tss;     // TSS
} task_t;

static task_t tasks[TASK_MAX_COUNT];
static u8 task_bitmap_data[DIV_ROUND_UP(TASK_MAX_COUNT, 8)];
static bitmap_t task_bitmap = {.data = task_bitmap_data};
size_t current_task_index;

static u32 get_cr3(void)
{
    u32 cr3;
    __asm__ volatile("mov %%cr3, %0" : "=r"(cr3));
    return cr3;
}

void task_manager_init(void)
{
    bitmap_init(&task_bitmap, TASK_MAX_COUNT);

    // 创建内核 TSS
    bitmap_allocate(&task_bitmap, 0);
    current_task_index = 0;
    task_t *task = tasks;
    tss_t *tss = &task->tss;
    tss->cr3 = get_cr3();

    // 设置 TSS 描述符
    task->gdt_index = gdt_set_desc_auto(&SEG_DESC((uintptr_t)tss, sizeof(tss_t) - 1, SEG_DESC_ACCESS(1, 0, 0, SEG_DESC_TYPE_TASK), SEG_DESC_FLAGS(0, 1, 0)));
    __asm__ __volatile__("ltr %0" : : "r"(task->gdt_index << 3)); // TI=0, RPL=0

    PR_INFO("Task Manager Initialized\n");
}

uintptr_t task_spawn(uintptr_t entry_point)
{
    bool found = false;
    size_t task_index = 0;
    for (size_t i = 0; i < task_bitmap.total; i++)
    {
        if (bitmap_get(&task_bitmap, i))
            continue;
        bitmap_allocate(&task_bitmap, i);
        task_index = i;
        found = true;
        break;
    }
    if (!found)
    {
        PANIC("No free task slot\n");
    }

    task_t *task = tasks + task_index;
    memset(task, 0, sizeof(task_t));
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
    return task_index;
}

static size_t find_next_task_index(void)
{
    // 从当前任务 + 1 开始查找
    for (size_t i = current_task_index + 1; i < task_bitmap.total; i++)
    {
        if (bitmap_get(&task_bitmap, i))
        {
            if (current_task_index != i)
                return i;
        }
    }
    // 从头开始查找，直到当前任务
    for (size_t i = 0; i < current_task_index; i++)
    {
        if (bitmap_get(&task_bitmap, i))
        {
            if (current_task_index != i)
                return i;
        }
    }
    return current_task_index;
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
    size_t next_task_index = find_next_task_index();
    if (next_task_index == current_task_index)
        return;
    task_t next_task = tasks[next_task_index];

    u16 tss_selector = (next_task.gdt_index << 3); // TI=0, RPL=0

    current_task_index = next_task_index;
    far_jump(tss_selector, 0);
}

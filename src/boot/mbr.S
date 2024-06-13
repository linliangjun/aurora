# BIOS Legacy MBR 引导代码
#
# Copyright 2024 linliangjun

    .extern setmode_16, putstrln_16, panic_16
    .globl  start

    .section .text.start
    .code16
start:
    # 通过跳转间接设置段寄存器 cs
    jmp     $0, $begin              # cs = 0

begin:
    # 设置段寄存器 ds，es，ss
    xor     %ax, %ax
    mov     %ax, %ds                # ds = 0
    mov     %ax, %es                # es = 0
    mov     %ax, %ss                # ss = 0

    # 设置栈顶
    mov     $start, %sp             # sp = 0x7c00

    # 保存驱动器号
    mov     %dl, driverno

    # 设置显示模式为 80 * 25 16 色文本模式
    push    $0
    push    $3
    call    setmode_16
    add     $4, %sp

    # 打印字符串
    push    $7
    push    $message
    call    putstrln_16
    add     $4, %sp

    # 加载 stage2（直接调用 BIOS 中断）
    mov     $0x20f, %ax             # ah 固定为 2；al 扇区数量
    mov     $0x7e00, %bx            # es:bx 缓冲区地址
    mov     $2, %cx                 # 低 6 位为扇区号（从 1 开始）；高 10 位为柱面号（从 0 开始）
    mov     driverno, %dl           # 驱动器号
    xor     %dh, %dh                # 磁头号
    int     $0x13

    # 加载成功，跳转到 stage2_start
    jnc     stage2_start

    # 加载失败，panic
    mov     %ah, %al                # 错误码，低 8 位
    mov     $1, %ah                 # 错误码，高 8 位
    push    %ax                     # 压入错误码
    call    panic_16

    .section .rodata
message:
    .string "Hello world!"

    .bss
    .comm   driverno, 1

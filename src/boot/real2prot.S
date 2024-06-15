# 实模式切换到保护模式函数，平坦内存模型
#
# void real2prot(uint32_t stack_top);
#
# 参数 stack_top：栈顶
#
# Copyright 2024 linliangjun

    .globl  real2prot

    .set    BOOT_CODE_SEG_SELE, (1 << 3)
    .set    BOOT_DATA_SEG_SELE, (2 << 3)

    # 段描述符宏
    .macro  SEG_DESC base:req, limit:req, S:req, type:req, DPL:req, G:req
    .word   \limit & 0xffff
    .word   \base & 0xffff
    .byte   (\base >> 16) & 0xff
    .byte   0x80 + ((\DPL & 3) << 5) + ((\S & 1) << 4) + (\type & 0xf)
    .byte   ((\G & 1) << 7) + 0x40 + ((\limit >> 16) & 0xf)
    .byte   (\base >> 24) & 0xff
    .endm

    .section .text.stage2
    .code16
real2prot:
    push    %bp
    mov     %sp, %bp

    # 计算返回地址，存放在 ecx 中
    xor     %edx, %edx
    mov     %cs, %dx
    shl     $4, %edx

    xor     %ecx, %ecx
    mov     2(%bp), %cx
    add     %edx, %ecx

    # 获取栈顶，存放在 edx 中
    mov     4(%bp), %edx

    cli                             # 关闭中断
    in      $0x92, %al              # 打开 A20
    or      $2, %al
    out     %al, $0x92
    lgdt    gdtdesc                 # 加载 GDT
    mov     %cr0, %eax              # 开启保护模式
    or      $1, %eax
    mov     %eax, %cr0
    jmp     $BOOT_CODE_SEG_SELE, $protect_entry

    .code32
protect_entry:
    # 设置段寄存器 ds，es，fs，gs，ss
    mov     $BOOT_DATA_SEG_SELE, %ax
    mov     %ax, %ds
    mov     %ax, %es
    mov     %ax, %fs
    mov     %ax, %gs
    mov     %ax, %ss

    # 设置 esp
    mov     %edx, %esp

    push    %ecx
    ret

    .section .rodata.stage2
gdt:
    .quad   0
    SEG_DESC base=0, limit=0xffffffff, S=1, type=0b1010, DPL=0, G=1
    SEG_DESC base=0, limit=0xffffffff, S=1, type=0b0010, DPL=0, G=1
gdtdesc:
    .word   . - gdt - 1
    .long   gdt

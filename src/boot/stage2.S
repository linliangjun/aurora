# stage2
#
# Copyright 2024 linliangjun

    .globl  stage2_entry

    .set    BOOT_CODE_SEG_SELE, (1 << 3)
    .set    BOOT_DATA_SEG_SELE, (2 << 3)

    # 段描述符宏
    .macro  seg_desc base:req, limit:req, S:req, type:req, DPL:req, G:req
    .word   \limit & 0xffff
    .word   \base & 0xffff
    .byte   (\base >> 16) & 0xff
    .byte   0x80 + ((\DPL & 3) << 5) + ((\S & 1) << 4) + (\type & 0xf)
    .byte   ((\G & 1) << 7) + 0x40 + ((\limit >> 16) & 0xf)
    .byte   (\base >> 24) & 0xff
    .endm

    .text
    .code16
stage2_entry:
    # 进入保护模式
    cli
    in      $0x92, %al
    or      $2, %al
    out     %al, $0x92
    lgdt    gdt_desc
    mov     %cr0, %eax
    or      $1, %eax
    mov     %eax, %cr0

    # 绝对远眺转，刷新段寄存器 cs 和流水线
    jmp     $BOOT_CODE_SEG_SELE, $protect_entry

    .code32
protect_entry:
    mov     $BOOT_DATA_SEG_SELE, %ax
    mov     %ax, %ds
    mov     %ax, %es
    mov     %ax, %fs
    mov     %ax, %gs
    mov     %ax, %ss
    mov     $stage2_entry, %esp

    # 跳转到 stage2_main
    jmp     stage2_main

    .section .rodata
gdt:
    .quad   0
    seg_desc base=0, limit=0xffffffff, S=1, type=0b1010, DPL=0, G=1
    seg_desc base=0, limit=0xffffffff, S=1, type=0b0010, DPL=0, G=1
gdt_desc:
    .word   . - gdt - 1
    .long   gdt

    .section .note.GNU-stack, "", @progbits

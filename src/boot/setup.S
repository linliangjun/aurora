.code16

.section .text

.global setup
setup:
    // 进入保护模式
    cli
    in $0x92, %al
    or $2, %al
    out %al, $0x92
    lgdt (gdt_ptr)
    mov %cr0, %eax
    or $1, %eax
    mov %eax, %cr0

    // 跳转到保护模式代码
    ljmp $8, $pe_code

desc_base:
    .quad 0
desc_code:
    .word 0xffff
    .byte 0, 0, 0
    .byte 0b10011010
    .byte 0b11001111
    .byte 0
desc_data:
    .word 0xffff
    .byte 0, 0, 0
    .byte 0b10010010
    .byte 0b11001111
    .byte 0
gdt_ptr:
    .word . - desc_base - 1
    .long desc_base

.code32

pe_code:
    // 初始化段
    mov $16, %ax
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs
    mov %ax, %ss
    mov $0x7c00, %esp

    // 打印字符串
    movl $(0xb8000 + 160), %ebx
    movb $'P', (%ebx)
    movb $'E', 2(%ebx)
    jmp .

.code16

.section .text

.extern setup
.global _start
_start:
    // 初始化段
    xor %ax, %ax
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %ss
    mov %ax, %sp

    // 设置显示模式
    mov $3, %ax
    int $0x10

    // 打印字符串
    mov $0x1301, %ax
    mov $7, %bx
    mov $13, %cx
    xor %dx, %dx
    mov $msg, %bp
    int $0x10

    // 加载并运行 setup
    // 我们约定 setup 在紧随其后的一个扇区中，并且占用 4 个扇区
    mov $0x204, %ax
    mov $0x7e00, %bx
    mov $2, %cx
    mov $0x80, %dx
    int $0x13
    jmp setup

msg:
    .string "Hello, world!"

.org 510
    .word 0xaa55

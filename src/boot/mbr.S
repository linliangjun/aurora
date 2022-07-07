.code16

.section .text

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

    jmp .

msg:
    .string "Hello, world!"

.org 510
    .word 0xaa55

.section .text

.global inb
inb:
    push %ebp
    mov %esp, %ebp
    mov 8(%ebp), %edx
    in %dx, %al
    leave
    ret

.global inw
inw:
    push %ebp
    mov %esp, %ebp
    mov 8(%ebp), %edx
    in %dx, %ax
    leave
    ret

.global outb
outb:
    push %ebp
    mov %esp, %ebp
    mov 8(%ebp), %edx
    mov 12(%ebp), %ax
    out %al, %dx
    leave
    ret

.global outw
outw:
    push %ebp
    mov %esp, %ebp
    mov 8(%ebp), %edx
    mov 12(%ebp), %ax
    out %ax, %dx
    leave
    ret

.section .text

.global load_kernel
load_kernel:
    // 函数调用
    push $0x10000
    push $200
    push $5
    call load_disk_data
    ljmp $8, $0x10000

//  加载第一通道主盘的数据，参数列表：unsigned int sector_num, unsigned char count, unsigned int buffer_addr
load_disk_data:
    push %ebp
    mov %esp, %ebp
    mov 12(%ebp), %eax
    mov $0x1f2, %dx
    out %al, %dx
    mov 8(%ebp), %eax
    mov $3, %ecx
.fill:
    inc %dx
    out %al, %dx
    shr $8, %eax
    loop .fill
    inc %dx
    shr $8, %eax
    or $0xe0, %al
    out %al, %dx
    inc %dx
    mov $0x20, %al
    out %al, %dx
    mov 12(%ebp), %ecx
    mov 16(%ebp), %edi
.wait:
    mov $0x1f7, %dx
    in %dx, %al
    and $0x88, %al
    cmp $8, %al
    jnz .wait
    push %ecx
    mov $256, %ecx
    mov $0x1f0, %dx
.read_word:
    in %dx, %ax
    stosw
    loop .read_word
    pop %ecx
    loop .wait
    leave
    ret

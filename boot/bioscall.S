/**
 * BIOS 中断调用
 *
 * Copyright 2024 linliangjun
 */

    .globl      int_10h_0eh, int_13h_02h, int_13h_08h, int_15h_e820h

    .text
int_10h_0eh:
.code32
    push        %ebp
    mov         %esp, %ebp

    // 进入实地址模式
    call        prot_to_real
    .code16

    // 调用 BIOS 中断 int 0x10/0x0e
    mov         8(%bp), %al
    mov         $0xe, %ah
    int         $0x10

    // 进入 32 位保护模式
    call        real_to_prot
    .code32
    leave
    ret

int_13h_02h:
    .code32
    push        %ebp
    mov         %esp, %ebp

    // 进入实地址模式
    call        prot_to_real
    .code16

    // 调用 BIOS 中断 int 0x13 0x02 读取磁盘扇区
    push        %bx
    mov         12(%bp), %bx
    mov         4(%bx), %ax
    mov         %ax, %es
    mov         3(%bx), %al
    mov         (%bx), %cx
    mov         2(%bx), %dh
    mov         8(%bp), %dl
    mov         6(%bx), %bx
    mov         $2, %ah
    int         $0x13

    // 保存错误码
    shr         $8, %ax
    push        %ax

    // 进入 32 位保护模式
    call        real_to_prot
    .code32

    // 恢复错误码
    pop         %ax
    leave
    ret

int_13h_08h:
    .code32
    push        %ebp
    mov         %esp, %ebp

    // 进入实地址模式
    call        prot_to_real
    .code16

    // 调用 BIOS 中断 int 0x13 0x08 读取驱动器参数
    push        %bx
    push        %di
    mov         8(%bp), %dl
    mov         $8, %ah
    int         $0x13

    // 若未出错，则填充结构体
    jc          1f
    mov         12(%bp), %bx
    mov         %cx, (%bx)
    mov         %dl, 2(%bx)
    pop         %di
    pop         %bx

1:
    // 保存错误码
    shr         $8, %ax
    push        %ax

    // 进入 32 位保护模式
    call        real_to_prot
    .code32

    // 恢复错误码
    pop         %ax
    leave
    ret

int_15h_e820h:
    .code32
    push        %ebp
    mov         %esp, %ebp

    // 进入实地址模式
    call        prot_to_real
    .code16

    // 调用 BIOS 中断 0x15/0xe820 获取内存映射信息
    push        %ebx
    push        %di
    mov         $0xe820, %eax
    mov         12(%bp), %bx
    mov         (%bx), %ebx
    mov         $20, %ecx
    mov         $0x534d4150, %edx
    mov         8(%bp), %di
    int         $0x15

    // 若未出错，则设置后续值，以及错误码（0）
    jc          1f
    mov         %ebx, %edx
    mov         12(%bp), %bx
    mov         %edx, (%bx)
    xor         %ax, %ax
    jmp         2f
1:
    mov         $1, %ax
2:
    pop         %di
    pop         %ebx

    // 保存错误码
    push        %ax

    // 进入 32 位保护模式
    call        real_to_prot
    .code32

    // 恢复错误码
    pop         %ax
    leave
    ret

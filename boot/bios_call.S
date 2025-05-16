/**
 * BIOS 中断调用
 *
 * Copyright 2025 linliangjun
 */

    .globl      int_10h_0eh, int_13h_02h, int_13h_08h, int_15h_e820h

    .text
int_10h_0eh:
    .code32
    push        %ebp
    mov         %esp, %ebp

    // 进入 16 位实地址模式
    push        $12
    call        prot_to_real
    .code16

    // 调用 BIOS 中断 int 0x10 0x0e
    mov         8(%bp), %al
    mov         $0xe, %ah
    int         $0x10

    // 进入 32 位保护模式
    push       $0
    call       real_to_prot
    .code32

    // 返回
    leave
    ret


int_13h_02h:
    .code32
    push        %ebp
    mov         %esp, %ebp

    // 进入 16 位实地址模式
    push        $32
    call        prot_to_real
    .code16

    // 调用 BIOS 中断 int 0x13 0x02
    push        %bx
    mov         $2, %ah
    mov         20(%bp), %al
    mov         12(%bp), %cx
    mov         16(%bp), %dh
    mov         8(%bp), %dl
    mov         24(%bp), %bx
    mov         %bx, %es
    mov         28(%bp), %bx
    int         $0x13
    pop         %bx

    // 保存错误码
    shr         $8, %ax
    push        %ax

    // 进入 32 位保护模式
    push       $2
    call       real_to_prot
    .code32

    // 返回
    pop        %ax
    leave
    ret


int_13h_08h:
    .code32
    push        %ebp
    mov         %esp, %ebp

    // 进入 16 位实地址模式
    push        $12
    call        prot_to_real
    .code16

    // 调用 BIOS 中断 int 0x13 0x08
    mov         $8, %ah
    mov         8(%bp), %dl
    int         $0x13

    // 保存错误码
    shr         $8, %ax
    push        %ax

    // 保存参数
    push        %cx
    shr         $8, %dx
    push        %dx

    // 进入 32 位保护模式
    push       $6
    call       real_to_prot
    .code32

    // 设置参数
    pop        %dx
    mov        16(%ebp), %eax
    mov        %dx, (%eax)

    pop        %cx
    mov        12(%ebp), %eax
    mov        %cx, (%eax)

    // 返回
    pop        %ax
    leave
    ret


int_15h_e820h:
    .code32
    push        %ebp
    mov         %esp, %ebp

    // 获取后续值
    mov         8(%ebp), %eax
    mov         (%eax), %eax
    push        %eax

    // 进入 16 位实地址模式
    push        $24
    call        prot_to_real
    .code16

    // 调用 BIOS 中断 int 0x15 0xe820
    push        %ebx
    push        %di
    mov         $0xe820, %eax
    mov         (%bp), %ebx
    mov         $20, %ecx
    mov         $0x534d4150, %edx
    mov         16(%bp), %di
    mov         %di, %es
    mov         20(%bp), %di
    int         $0x15

    // 将后续值保存在 edx 中
    mov         %ebx, %edx

    pop         %di
    pop         %ebx

    // 保存错误码
    jc          1f
    xor         %ax, %ax
    jmp         2f
1:
    mov         $1, %ax
2:
    push        %ax

    // 保存参数
    push        %edx

    // 进入 32 位保护模式
    push       $6
    call       real_to_prot
    .code32

    // 设置参数
    pop        %edx
    mov        8(%ebp), %eax
    mov        %edx, (%eax)

    // 返回
    pop        %ax
    leave
    ret

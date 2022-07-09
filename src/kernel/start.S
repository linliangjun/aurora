
.section .text

.extern kernel_init
.global _start
_start:
    // 打印字符串
    movl $(0xb8000 + 320), %ebx
    movb $'K', (%ebx)
    movb $'N', 2(%ebx)

    // 函数调用
    call kernel_init
    jmp .

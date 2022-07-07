.code16

.section .text

.global _start
_start:
    jmp .

.org 510
    .word 0xaa55

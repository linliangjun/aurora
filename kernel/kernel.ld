OUTPUT_FORMAT(elf32-i386)
OUTPUT_ARCH(i386)

PHDRS
{
    text.init PT_LOAD;
    text PT_LOAD;
    rodata PT_LOAD;
    data PT_LOAD;
    bss PT_LOAD;
}

ENTRY(_start)

SECTIONS
{
    . = 0x100000;

    .text.init : {
        *(.text.init);
    } : text.init

    . = ALIGN(4096);

    __kernel_phys_start = .;

    . = 0xc0000000;

    __kernel_start = .;

    .text : AT(ADDR(.text) - __kernel_start + __kernel_phys_start) {
        *(.text);
    } : text

    . = ALIGN(4096);

    .rodata : AT(ADDR(.rodata) - __kernel_start + __kernel_phys_start) {
        *(.rodata);
    } : rodata

    . = ALIGN(4096);

    .data : AT(ADDR(.data) - __kernel_start + __kernel_phys_start) {
        *(.data);
    } : data

    . = ALIGN(4096);

    .bss : AT(ADDR(.bss) - __kernel_start + __kernel_phys_start) {
        *(.bss);
    } : bss

    __kernel_end = .;

    /DISCARD/ : {
        *(.note.gnu.property);
    }
}

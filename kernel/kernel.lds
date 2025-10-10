OUTPUT_FORMAT(elf32-i386)
OUTPUT_ARCH(i386)

PHDRS
{
    hhk_init_text PT_LOAD;
    hhk_init_bss PT_LOAD;
    text PT_LOAD;
    rodata PT_LOAD;
    data PT_LOAD;
    bss PT_LOAD;
}

ENTRY(_start)

SECTIONS
{
    . = 0x100000;

    .hhk_init_text : {
        *(.hhk_init.text);
    } : hhk_init_text

    . = ALIGN(4K);

    .hhk_init_bss : {
        *(.hhk_init.bss);
    } : hhk_init_bss

    . = ALIGN(4K);

    __kernel_phys_start = .;

    . = 0xC0000000;
    __kernel_start = .;

    .text : AT(__kernel_phys_start) {
        *(.text);
    } : text

    . = ALIGN(4K);

    .rodata : AT(__kernel_phys_start + (ADDR(.rodata) - __kernel_start)) {
        *(.rodata);
    } : rodata

    . = ALIGN(4K);

    .data : AT(__kernel_phys_start + (ADDR(.data) - __kernel_start)) {
        *(.data);
    } : data

    . = ALIGN(4K);

    .bss : AT(__kernel_phys_start + (ADDR(.bss) - __kernel_start)) {
        *(.bss);
    } : bss

    __kernel_end = .;
    __kernel_phys_end = __kernel_phys_start + (__kernel_end - __kernel_start);

    /DISCARD/ : {
        *(.note.gnu.property);
    }
}

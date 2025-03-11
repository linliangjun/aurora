OUTPUT_FORMAT(elf32-i386)
OUTPUT_ARCH(i386)

PHDRS
{
    text PT_LOAD;
    rodata PT_LOAD;
    data PT_LOAD;
    bss PT_LOAD;
}

ENTRY(_start)

SECTIONS
{
    . = 0x100000;

    __kernel_start = .;

    .text : {
        *(.text);
    } : text

    . = ALIGN(16);

    .rodata : {
        *(.rodata);
    } : rodata

    . = ALIGN(16);

    .data : {
        *(.data);
    } : data

    . = ALIGN(16);

    .bss : {
        *(.bss);
    } : bss

    __kernel_end = .;

    /DISCARD/ : {
        *(.note.gnu.property);
    }
}

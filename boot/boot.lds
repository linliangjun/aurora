OUTPUT_FORMAT(elf32-i386)
OUTPUT_ARCH(i386)

ENTRY(_start)

PHDRS
{
    text PT_LOAD;
    data PT_LOAD;
    rodata PT_LOAD;
    bss PT_LOAD;
}

SECTIONS
{
    . = 0x7c00;

    .text : {
        *(.text.startup);
        *(.text);
    } : text

    . = ALIGN(16);

    .data : {
        *(.data);
    } : data

    . = ALIGN(16);

    .rodata : {
        *(.rodata);
    } : rodata

    . = ALIGN(16);

    .bss : {
        *(.bss);
    } : bss

    /DISCARD/ : {
        *(.note.gnu.property);
    }
}

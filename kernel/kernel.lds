OUTPUT_FORMAT(elf32-i386)
OUTPUT_ARCH(i386)

PHDRS
{
    text PT_LOAD;
    rodata PT_LOAD;
}

ENTRY(_start)

SECTIONS
{
    . = 0x100000;

    .text : {
        *(.text.startup);
        *(.text);
    } : text

    . = ALIGN(4K);

    .rodata : {
        *(.rodata);
    } : rodata

    /DISCARD/ : {
        *(.note.gnu.property);
    }
}

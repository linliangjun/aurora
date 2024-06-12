OUTPUT_FORMAT(elf32-i386)
OUTPUT_ARCH(i386)

PHDRS
{
    text PT_LOAD;
    rodata PT_LOAD;
}

ENTRY(start)

SECTIONS
{
    . = 0x7c00;

    .text : {
        *(.text);
    } : text

    . = ALIGN(16);

    .rodata : {
        *(.rodata);
    } : rodata

    . = 0x7dfe;

    .sig.mbr : {
        SHORT(0xaa55);
    } : rodata

    /DISCARD/ : {
        *(.note.gnu.property);
    }
}

OUTPUT_FORMAT(elf32-i386)
OUTPUT_ARCH(i386)

PHDRS
{
    text PT_LOAD;
    rodata PT_LOAD;
}

SECTIONS
{
    . = 0x7e00;

    .text : {
        *(.text.startup);
        *(.text);
    } : text

    . = ALIGN(16);

    .rodata : {
        *(.rodata);
    } : rodata

    /DISCARD/ : {
        *(.note.gnu.property);
    }
}

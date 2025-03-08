OUTPUT_FORMAT(elf32-i386)
OUTPUT_ARCH(i386)

PHDRS
{
    text PT_LOAD;
}

SECTIONS
{
    . = 0x7e00;

    .text : {
        *(.text);
    } : text

    /DISCARD/ : {
        *(.note.gnu.property);
    }
}

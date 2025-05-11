OUTPUT_FORMAT(elf32-i386)
OUTPUT_ARCH(i386)

PHDRS
{
    text PT_LOAD;
}

SECTIONS
{
    . = 0x7c00;

    .text : {
        *(.text);
    } : text

    /DISCARD/ : {
        *(.note.gnu.property);
    }
}

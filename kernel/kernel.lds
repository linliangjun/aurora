OUTPUT_FORMAT(elf32-i386)
OUTPUT_ARCH(i386)

PHDRS
{
    text PT_LOAD;
}

ENTRY(_start)

SECTIONS
{
    . = 0x100000;

    .text : {
        *(.text);
    } : text

    /DISCARD/ : {
        *(.note.gnu.property);
    }
}

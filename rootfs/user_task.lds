OUTPUT_FORMAT(elf32-i386)
OUTPUT_ARCH(i386)

PHDRS
{
    text PT_LOAD;
    rodata PT_LOAD;
}

ENTRY(user_task)

SECTIONS
{
    . = 0x800000;

    .text : {
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

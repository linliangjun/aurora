OUTPUT_FORMAT(elf32-i386)
OUTPUT_ARCH(i386)

PHDRS
{
    text PT_LOAD;
    rodata PT_LOAD;
    text.stage2 PT_LOAD;
    rodata.stage2 PT_LOAD;
    bss PT_LOAD;
}

ENTRY(start)

SECTIONS
{
    . = 0x7c00;

    .text : {
        *(.text.start);
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

    .text.stage2 : {
        *(.text.stage2);
    } : text.stage2

    . = ALIGN(16);

    .rodata.stage2 : {
        *(.rodata.stage2);
    } : rodata.stage2

    . = ALIGN(16);

    .bss : {
        *(.bss);
    } : bss

    /DISCARD/ : {
        *(.note.gnu.property);
    }
}

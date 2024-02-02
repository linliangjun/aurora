OUTPUT_FORMAT(elf32-i386)
OUTPUT_ARCH(i386)

PHDRS
{
    mbr_text PT_LOAD;
    mbr_rodata PT_LOAD;
}

SECTIONS
{
    . = 0x7c00;

    .mbr_text : {
        ./build/bin/boot/start.o(.text);
    } : mbr_text

    . = 0x7dfe;

    .mbr_sig : {
        SHORT(0xaa55);
    } : mbr_rodata
}

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
        ./build/bin/boot/puts_16.o(.text);
        ./build/bin/boot/strlen_16.o(.text);
    } : mbr_text

    . = ALIGN(16);

    .mbr_rodata : {
        ./build/bin/boot/start.o(.rodata);
    } : mbr_rodata

    . = 0x7dfe;

    .mbr_sig : {
        SHORT(0xaa55);
    } : mbr_rodata
}

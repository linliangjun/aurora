OUTPUT_FORMAT(elf32-i386)
OUTPUT_ARCH(i386)

ENTRY(_start)

PHDRS
{
    text.startup PT_LOAD;
    text PT_LOAD;
    rodata PT_LOAD;
    data PT_LOAD;
    bss PT_LOAD;
}

SECTIONS
{
    . = 0x10000;

    .text.startup : {
        *(.text.startup);
        . = ALIGN(4096);
    } : text.startup

    . = 0xc0000000;

    __kernel_start = .;

    .text : AT(ADDR(.text) - 0xc0000000 + 0x100000) {
        *(.text);
    } : text

    . = ALIGN(4096);

    .rodata : AT(ADDR(.rodata) - 0xc0000000 + 0x100000) {
        *(.rodata);
    } : rodata

    . = ALIGN(4096);

    .data : AT(ADDR(.data) - 0xc0000000 + 0x100000) {
        *(.data);
    } : data

    . = ALIGN(4096);

    .bss : AT(ADDR(.bss) - 0xc0000000 + 0x100000) {
        *(.bss);
    } : bss

    __kernel_end = .;

    /DISCARD/ : {
        *(.note.gnu.property);
    }
}

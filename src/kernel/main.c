#include <io.h>

void kernel_init()
{
    char msg[] = "Kernel initializing...";
    char* ptr =(char*)(0xb8000 + 480);
    for (int i = 0; i < sizeof(msg); i++)
    {
        ptr[i * 2] = msg[i];
    }

    unsigned short pos = 240 + sizeof(msg) - 1;
    outb(0x3d4, 0xf);
    outb(0x3d5, pos);
    outb(0x3d4, 0xe);
    outb(0x3d5, pos >> 8);
}

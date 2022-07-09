

void kernel_init()
{
    char msg[] = "Kernel initializing...";
    char* ptr =(char*)(0xb8000 + 480);
    for (int i = 0; i < sizeof(msg); i++)
    {
        ptr[i * 2] = msg[i];
    }
}

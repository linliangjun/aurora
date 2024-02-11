#include <string.h>

void *memcpy(void *dest, const void *src, size_t n)
{
    int8_t *pdest = (int8_t *)dest, *psrc = (int8_t *)src;
    while (n--)
    {
        *pdest++ = *psrc++;
    }
    return dest;
}

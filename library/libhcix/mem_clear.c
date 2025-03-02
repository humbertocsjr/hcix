#include <hcix.h>

void *mem_clear(void *dest, size_t size)
{
    char *ptr = dest;
    for(size_t i = 0; i < size; i++)
    {
        ptr[i] = 0;
    }
    return dest;
}
#include <hcix.h>

char *mem_copy(char *dest, char *src, size_t capacity)
{
    for(size_t i = 0; i < capacity; i++)
    {
        dest[i] = src[i];
    }
    return dest;
}
#include <hcix.h>

char *string_copy(char *dest, char *src, size_t capacity)
{
    for(size_t i = 0; i < capacity; i++)
    {
        dest[i] = src[i];
        if(src[i] == 0) break;
    }
    return dest;
}
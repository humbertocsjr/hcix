#include <hcix.h>

int string_equal(char *dest, char *src, size_t capacity)
{
    for(size_t i = 0; i < capacity; i++)
    {
        if(dest[i] != src[i]) return 0;
        if(src[i] == 0) return 1;
    }
    return 1;
}
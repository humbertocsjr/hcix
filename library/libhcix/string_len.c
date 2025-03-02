#include <hcix.h>

size_t string_len(char *src, size_t capacity)
{
    for(size_t i = 0; i < capacity; i++)
    {
        if(src[i] == 0) return i;
    }
    return capacity;
}
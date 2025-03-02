#include <hcix.h>

void *mem_set_words(void *dest, uint16_t value, size_t size)
{
    uint16_t *ptr = dest;
    for(size_t i = 0; i < size; i++)
    {
        ptr[i] = value;
    }
    return dest;
}
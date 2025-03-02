#include <hcix.h>

uint32_t string_to_uint32(char *str, size_t capacity)
{
    uint32_t value = 0;
    for(size_t i = 0; i < capacity; i++)
    {
        if(str[i] >= '0' && str[i] <= '9')
        {
            value *= 10;
            value += str[i] - '0';
        }
        else if(str[i] != '_')
        {
            return value;
        }
    }
    return value;
}
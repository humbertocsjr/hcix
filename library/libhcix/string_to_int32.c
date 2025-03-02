#include <hcix.h>

int32_t string_to_int32(char *str, size_t capacity)
{
    int32_t value = 0;
    int negative = 0;
    size_t i = 0;
    if(str[0] == '-')
    {
        negative = 1;
        i++;
    }
    for(; i < capacity; i++)
    {
        if(str[i] >= '0' && str[i] <= '9')
        {
            value *= 10;
            value += str[i] - '0';
        }
        else if(str[i] != '_')
        {
            break;
        }
    }
    if(negative) value = -value;
    return value;
}

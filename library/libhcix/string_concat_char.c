#include <hcix.h>

char *string_concat_char(char *dest, char c, size_t capacity)
{
    for(size_t i = 0; i < capacity; i++)
    {
        if(dest[i] == 0 && (i+1) < capacity)
        {
            dest[i] = c;
            dest[i+1] = 0;
            break;
        }
    }
    return dest;
}
#include <hcix.h>

int char_is_space(char c)
{
    return c == ' ' | c == '\t' | c == '\v' | c == '\r';
}
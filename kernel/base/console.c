#include "hcix.h"

static device_t *_console = 0;

status_t console_set_phys(device_t *dev)
{
    status_t st;
    if(!_console)
    {
        _console = dev;
        device_init(_console);
        SET_STATUS(st, STATUS_OK, 0, 0);
    }
    else 
    {
        SET_STATUS(st, STATUS_ALREADY_EXISTS, 0, 0);
    }
    return st;
}

void console_write_const(const char *str)
{
    device_write(_console, str, string_len(str, 2048));
}

void console_write_uint32(uint32_t value)
{
    uint32_t next;
    char tmp[2];
    next = value / 10;
    if(next) console_write_uint32(next);
    tmp[0] = (value % 10) + '0';
    tmp[1] = 0;
    console_write_const(tmp);
}

void console_write_hex32(uint32_t value)
{
    char tmp[9];
    char *hex = "0123456789ABCDEF";
    tmp[0] = hex[(value >> 28) & 0xf];
    tmp[1] = hex[(value >> 24) & 0xf];
    tmp[2] = hex[(value >> 20) & 0xf];
    tmp[3] = hex[(value >> 16) & 0xf];
    tmp[4] = hex[(value >> 12) & 0xf];
    tmp[5] = hex[(value >> 8) & 0xf];
    tmp[6] = hex[(value >> 4) & 0xf];
    tmp[7] = hex[(value) & 0xf];
    tmp[8] = 0;
    console_write_const(tmp);
}

uint16_t console_get_width()
{
    status_t st;
    st = device_get_conf(_console, CONSOLE_CONF_WIDTH);
    return GET_VALUE(st, 0);
}

uint16_t console_get_height()
{
    status_t st;
    st = device_get_conf(_console, CONSOLE_CONF_HEIGHT);
    return GET_VALUE(st, 0);
}

uint16_t console_get_x()
{
    status_t st;
    st = device_get_conf(_console, CONSOLE_CONF_X);
    return GET_VALUE(st, 0);
}

uint16_t console_get_y()
{
    status_t st;
    st = device_get_conf(_console, CONSOLE_CONF_Y);
    return GET_VALUE(st, 0);
}

void console_set_position(uint16_t x, uint16_t y)
{
    status_t st;
    st = device_set_conf(_console, CONSOLE_CONF_Y, y);
    st = device_set_conf(_console, CONSOLE_CONF_X, x);
}
#include <hcix.h>

enum
{
    PARAM_X,
    PARAM_Y,
    PARAM_WIDTH,
    PARAM_HEIGHT,
    PARAM_COLORS
};

static void fix_xy(device_t *dev)
{
    uint16_t *buffer = dev->buffer;
    while(dev->params[PARAM_X] >= dev->params[PARAM_WIDTH])
    {
        dev->params[PARAM_X] -= dev->params[PARAM_WIDTH];
        dev->params[PARAM_Y]++;
    }
    while(dev->params[PARAM_Y] >= dev->params[PARAM_HEIGHT])
    {
        dev->params[PARAM_Y]--;
        mem_copy(&buffer[0], &buffer[dev->params[PARAM_WIDTH]], dev->params[PARAM_WIDTH] * (dev->params[PARAM_HEIGHT] - 1));
        mem_set_words(&buffer[dev->params[PARAM_WIDTH] * (dev->params[PARAM_HEIGHT] - 1)], dev->params[PARAM_COLORS] | ' ', dev->params[PARAM_WIDTH]);
    }
}

static void update_xy(device_t *dev)
{
    int pos = dev->params[PARAM_X] + (dev->params[PARAM_Y] * dev->params[PARAM_WIDTH]);
    io_write_uint8(0x3d4, 0xf);
    io_write_uint8(0x3d5, pos & 0xff);
    io_write_uint8(0x3d4, 0xe);
    io_write_uint8(0x3d5, (pos>>8) & 0xff);
}

static status_t init(device_t *dev)
{
    status_t st;
    uint16_t *buffer = dev->buffer;
    mem_set_words(buffer, dev->params[PARAM_COLORS] | ' ', dev->buffer_size);
    update_xy(dev);
    SET_STATUS(st, STATUS_OK, 0, 0);
    return st;
}

static status_t write(device_t *dev, void *data, size_t size)
{
    status_t st;
    char *ptr = data;
    uint16_t *buffer = dev->buffer;
    fix_xy(dev);
    for(size_t i = 0; i < size; i++)
    {
        int pos = dev->params[PARAM_X] + (dev->params[PARAM_Y] * dev->params[PARAM_WIDTH]);
        switch(ptr[i])
        {
            case '\n':
                dev->params[PARAM_X] = 0;
                dev->params[PARAM_Y]++;
                break;
            case '\r':
                dev->params[PARAM_X] = 0;
                break;
            default:
                buffer[pos] = dev->params[PARAM_COLORS] | (ptr[i] & 0xff);
                dev->params[PARAM_X]++;
                break;
        }
        fix_xy(dev);
        update_xy(dev);
    }
    SET_STATUS(st, STATUS_OK, 0, 0);
    return st;
}

static status_t set_conf(struct device_t *dev, int id, int value)
{
    status_t st;
    switch(id)
    {
        case CONSOLE_CONF_Y:
            dev->params[PARAM_Y] = value < 0 ? 0 : value;
            fix_xy(dev);
            update_xy(dev);
            SET_STATUS(st, STATUS_OK, 0, dev->params[PARAM_Y]);
            return st;
        case CONSOLE_CONF_X:
            dev->params[PARAM_X] = value < 0 ? 0 : value;
            fix_xy(dev);
            update_xy(dev);
            SET_STATUS(st, STATUS_OK, 0, dev->params[PARAM_X]);
            return st;
        case CONSOLE_CONF_FOREGROUND:
            dev->params[PARAM_COLORS] &= 0xf000;
            dev->params[PARAM_COLORS] |= (value << 8) & 0xf00;
            SET_STATUS(st, STATUS_OK, 0, (dev->params[PARAM_COLORS] >> 12) & 0xf);
            return st;
        case CONSOLE_CONF_BACKGROUND:
            dev->params[PARAM_COLORS] &= 0x0f00;
            dev->params[PARAM_COLORS] |= (value << 12) & 0xf000;
            SET_STATUS(st, STATUS_OK, 0, (dev->params[PARAM_COLORS] >> 8) & 0xf);
            return st;
    }
    SET_STATUS(st, STATUS_NOT_FOUND, 0, 0);
    return st;
}

static status_t get_conf(struct device_t *dev, int id)
{
    status_t st;
    switch(id)
    {
        case CONSOLE_CONF_Y:
            SET_STATUS(st, STATUS_OK, 0, dev->params[PARAM_Y]);
            return st;
        case CONSOLE_CONF_X:
            SET_STATUS(st, STATUS_OK, 0, dev->params[PARAM_X]);
            return st;
        case CONSOLE_CONF_WIDTH:
            SET_STATUS(st, STATUS_OK, 0, dev->params[PARAM_WIDTH]);
            return st;
        case CONSOLE_CONF_HEIGHT:
            SET_STATUS(st, STATUS_OK, 0, dev->params[PARAM_HEIGHT]);
            return st;
        case CONSOLE_CONF_FOREGROUND:
            SET_STATUS(st, STATUS_OK, 0, (dev->params[PARAM_COLORS] >> 12) & 0xf);
            return st;
        case CONSOLE_CONF_BACKGROUND:
            SET_STATUS(st, STATUS_OK, 0, (dev->params[PARAM_COLORS] >> 8) & 0xf);
            return st;
    }
    SET_STATUS(st, STATUS_NOT_FOUND, 0, 0);
    return st;
}

void pcvideotext_register()
{
    status_t st;
    device_t *dev;
    st = device_alloc(DEVICE_CONSOLE, "cgatext");
    if(OK(st))
    {
        dev = st.pointer;
        dev->write = write;
        dev->init = init;
        dev->get_conf = get_conf;
        dev->set_conf = set_conf;
        dev->params[PARAM_X] = 0;
        dev->params[PARAM_Y] = 0;
        dev->params[PARAM_HEIGHT] = 25;
        dev->params[PARAM_WIDTH] = 80;
        dev->params[PARAM_COLORS] = 0x300;
        dev->buffer = 0xb8000;
        dev->buffer_size = 2000;
    }
    st = device_alloc(DEVICE_CONSOLE, "mdatext");
    if(OK(st))
    {
        dev = st.pointer;
        dev->write = write;
        dev->init = init;
        dev->get_conf = get_conf;
        dev->set_conf = set_conf;
        dev->params[PARAM_X] = 0;
        dev->params[PARAM_Y] = 0;
        dev->params[PARAM_HEIGHT] = 25;
        dev->params[PARAM_WIDTH] = 80;
        dev->params[PARAM_COLORS] = 0x700;
        dev->buffer = 0xa0000;
        dev->buffer_size = 2000;
    }
}

#include <hcix.h>

#define DEVICES_MAX 512
static device_t _devices[DEVICES_MAX];

void device_register()
{
    for(uint32_t i = 0; i < DEVICES_MAX; i++)
    {
        mem_clear(&_devices[i], sizeof(device_t));
        _devices[i].id = i;
        _devices[i].type = DEVICE_NONE;
        string_copy(_devices[i].name, "", DEVICE_NAME_SIZE);
    }
}

status_t device_alloc(device_type_t type, char *name)
{
    status_t st;
    for(uint32_t i = 0; i < DEVICES_MAX; i++)
    {
        if(_devices[i].type == DEVICE_NONE)
        {
            string_copy(_devices[i].name, name, DEVICE_NAME_SIZE);
            _devices[i].type = type;
            SET_STATUS(st, STATUS_OK, &_devices[i], 0);
            return st;
        }
    }
    SET_STATUS(st, STATUS_CAPACITY_OVERFLOW, 0, 0);
    return st;
}

status_t device_find_by_name(char *name)
{
    status_t st;
    for(uint32_t i = 0; i < DEVICES_MAX; i++)
    {
        if(_devices[i].type != DEVICE_NONE && string_equal(_devices[i].name, name, DEVICE_NAME_SIZE))
        {
            SET_STATUS(st, STATUS_OK, &_devices[i], 0);
            return st;
        }
    }
    SET_STATUS(st, STATUS_NOT_FOUND, 0, 0);
    return st;
}

status_t device_init(device_t *dev)
{
    status_t st;
    if(!dev)
    {
        SET_STATUS(st, STATUS_INVALID_POINTER, 0, 0);
        return st;
    }
    if(!dev->init)
    {
        SET_STATUS(st, STATUS_NOT_IMPLEMENTED, 0, 0);
        return st;
    }
    return dev->init(dev);
}

status_t device_init_by_name(char *name)
{
    status_t st;
    st = device_find_by_name(name);
    if(ERROR(st)) return st;
    return device_init(st.pointer);
}

status_t device_write(device_t *dev, void *data, size_t size)
{
    status_t st;
    if(!dev)
    {
        SET_STATUS(st, STATUS_INVALID_POINTER, 0, 0);
        return st;
    }
    if(!dev->write)
    {
        SET_STATUS(st, STATUS_NOT_IMPLEMENTED, 0, 0);
        return st;
    }
    return dev->write(dev, data, size);
}

status_t device_write_by_name(char *name, void *data, size_t size)
{
    status_t st;
    st = device_find_by_name(name);
    if(ERROR(st)) return st;
    return device_write(st.pointer, data, size);
}

status_t device_read(device_t *dev, void *data, size_t size)
{
    status_t st;
    if(!dev)
    {
        SET_STATUS(st, STATUS_INVALID_POINTER, 0, 0);
        return st;
    }
    if(!dev->read)
    {
        SET_STATUS(st, STATUS_NOT_IMPLEMENTED, 0, 0);
        return st;
    }
    return dev->read(dev, data, size);
}

status_t device_read_by_name(char *name, void *data, size_t size)
{
    status_t st;
    st = device_find_by_name(name);
    if(ERROR(st)) return st;
    return device_read(st.pointer, data, size);
}

status_t device_set_conf(device_t *dev, int id, int value)
{
    status_t st;
    if(!dev)
    {
        SET_STATUS(st, STATUS_INVALID_POINTER, 0, 0);
        return st;
    }
    if(!dev->set_conf)
    {
        SET_STATUS(st, STATUS_NOT_IMPLEMENTED, 0, 0);
        return st;
    }
    return dev->set_conf(dev, id, value);
}

status_t device_set_conf_by_name(char *name, int id, int value)
{
    status_t st;
    st = device_find_by_name(name);
    if(ERROR(st)) return st;
    return device_set_conf(st.pointer, id, value);
}

status_t device_get_conf(device_t *dev, int id)
{
    status_t st;
    if(!dev)
    {
        SET_STATUS(st, STATUS_INVALID_POINTER, 0, 0);
        return st;
    }
    if(!dev->get_conf)
    {
        SET_STATUS(st, STATUS_NOT_IMPLEMENTED, 0, 0);
        return st;
    }
    return dev->get_conf(dev, id);
}

status_t device_get_conf_by_name(char *name, int id)
{
    status_t st;
    st = device_find_by_name(name);
    if(ERROR(st)) return st;
    return device_get_conf(st.pointer, id);
}

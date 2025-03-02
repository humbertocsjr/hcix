#include <hcix.h>

extern char _start_allcode__;
extern char _stop_allcode__;
extern char _start_alldata__;
extern char _stop_alldata__;

enum
{
    MEMMGR_PARAM_LOWMEM,
    MEMMGR_PARAM_HIGHMEM
};

#define ALIGN_UP(ptr) (((size_t)ptr + 0xfff) & 0xfffff000)

uint8_t *_memory_map = 0;
size_t _memory_map_size = 0; 

enum
{
    MEMORY_EMPTY,
    MEMORY_RESERVED,
    MEMORY_IN_USE,
    MEMORY_MULTIBLOCK_IN_USE,
    MEMORY_LAST_MULTIBLOCK_IN_USE,
    MEMORY_KERNEL
}; 

static status_t init(device_t *dev)
{
    status_t st;
    size_t first_map_page = 0;
    size_t last_map_page = 0;
    size_t first_kernel_page = (size_t)&_start_allcode__ / 4096;
    size_t last_kernel_page = (size_t)&_stop_alldata__ / 4096;
    size_t first_low_reserved_page = dev->params[MEMMGR_PARAM_LOWMEM] / 4;
    size_t last_low_reserved_page = (1024 * 1024 - 1) / 4096;
    size_t first_high_reserved_page = (15 * 1024 * 1024) / 4096;
    size_t last_high_reserved_page = (16 * 1024 * 1024 - 1) / 4096;
    _memory_map = ALIGN_UP(&_stop_alldata__);
    _memory_map_size = (1024 + dev->params[MEMMGR_PARAM_HIGHMEM]) / 4;
    first_map_page = (size_t)_memory_map / 4096;
    last_map_page = (_memory_map_size / 4096) + ((size_t)_memory_map / 4096);
    for(size_t i = 0; i < _memory_map_size; i++)
    {
        if(i == 0)
        {
            _memory_map[i] = MEMORY_RESERVED;
        }
        else if(first_kernel_page <= i & i <= last_kernel_page)
        {
            _memory_map[i] = MEMORY_KERNEL;
        }
        else if(first_map_page <= i & i <= last_map_page)
        {
            _memory_map[i] = MEMORY_KERNEL;
        }
        else if(first_low_reserved_page <= i & i <= last_low_reserved_page)
        {
            _memory_map[i] = MEMORY_RESERVED;
        }
        else if(first_high_reserved_page <= i & i <= last_high_reserved_page)
        {
            _memory_map[i] = MEMORY_RESERVED;
        }
        else
        {
            _memory_map[i] = MEMORY_EMPTY;
        }
    }

    return st;
}

static status_t read(device_t *dev, void *data, size_t size)
{
    status_t st;
    size_t pages = ALIGN_UP(size) / 4096;
    size_t first = 0;
    size_t found = 0;
    uint8_t *ptr;
    for(size_t i = 0; i < _memory_map_size; i++)
    {
        if(_memory_map[i] == MEMORY_EMPTY)
        {
            if(first)
            {
                found++;
            }
            else
            {
                first = i;
                found = 1;
            }
            if(found >= pages)
            {
                for(size_t j = 0; j < found; j++)
                {
                    _memory_map[first+j] = ((j+1) == found) ? MEMORY_LAST_MULTIBLOCK_IN_USE : MEMORY_MULTIBLOCK_IN_USE;
                }
                ptr = (size_t)(((size_t)first) * 4096);
                mem_clear(ptr, pages * 4096);
                *(void **)data = ptr;
                SET_STATUS(st, STATUS_OK, ptr, 0);
                return st;
            }
        }
        else
        {
            first = 0;
            found = 0;
        }
    }
    SET_STATUS(st, STATUS_CAPACITY_OVERFLOW, 0, 0);
    return st;
}

static status_t set_conf(struct device_t *dev, int id, int value)
{
    status_t st;
    switch(id)
    {
        case MEMMGR_CONF_LOWMEM_CAPACITY:
            if(dev->params[MEMMGR_PARAM_LOWMEM])
            {
                SET_STATUS(st, STATUS_ALREADY_EXISTS, 0, dev->params[MEMMGR_PARAM_LOWMEM]);
            }
            else
            {
                dev->params[MEMMGR_PARAM_LOWMEM] = value;
                SET_STATUS(st, STATUS_OK, 0, dev->params[MEMMGR_PARAM_LOWMEM]);
            }
            return st;
        case MEMMGR_CONF_HIGHMEM_CAPACITY:
            if(dev->params[MEMMGR_PARAM_HIGHMEM])
            {
                SET_STATUS(st, STATUS_ALREADY_EXISTS, 0, dev->params[MEMMGR_PARAM_HIGHMEM]);
            }
            else
            {
                dev->params[MEMMGR_PARAM_HIGHMEM] = value;
                SET_STATUS(st, STATUS_OK, 0, dev->params[MEMMGR_PARAM_HIGHMEM]);
            }
            return st;
    }
    SET_STATUS(st, STATUS_NOT_FOUND, 0, 0);
    return st;
}

static status_t get_conf(struct device_t *dev, int id)
{
    status_t st;
    size_t count = 0;
    switch(id)
    {
        case MEMMGR_CONF_LOWMEM_CAPACITY:
            SET_STATUS(st, STATUS_OK, 0, dev->params[MEMMGR_PARAM_LOWMEM]);
            return st;
        case MEMMGR_CONF_HIGHMEM_CAPACITY:
            SET_STATUS(st, STATUS_OK, 0, dev->params[MEMMGR_PARAM_HIGHMEM]);
            return st;
        case MEMMGR_CONF_FREE_PAGES:
            for(size_t i = 0 ; i < _memory_map_size; i++)
            {
                if(_memory_map[i] == MEMORY_EMPTY) count++;
            }
            SET_STATUS(st, STATUS_OK, 0, count);
            return st;
        case MEMMGR_CONF_TOTAL_PAGES:
            SET_STATUS(st, STATUS_OK, 0, _memory_map_size);
            return st;
    }
    SET_STATUS(st, STATUS_NOT_FOUND, 0, 0);
    return st;
}


void memmgr_register()
{
    device_t *dev;
    status_t st;

    st = device_alloc(DEVICE_MEMORY_MANAGER, "memmgr");
    if(OK(st))
    {
        dev = st.pointer;
        dev->init = &init;
        dev->read = read;
        dev->get_conf = &get_conf;
        dev->set_conf = &set_conf;
        dev->params[MEMMGR_PARAM_HIGHMEM] = 0;
        dev->params[MEMMGR_PARAM_LOWMEM] = 0;
    }
}

void *memmgr_alloc(size_t size)
{
    void *ptr = 0;
    status_t st;
    st = device_read_by_name("memmgr", &ptr, size);
    return ptr;
}
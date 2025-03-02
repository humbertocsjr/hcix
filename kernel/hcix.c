
/*

Kernel Args:

 -console [device]              set physical console device
 -memory [size in MiB]          set total memory size in MiB

*/

#include <hcix.h>

#define ADD_MODULE(module) extern void module##_register(); module##_register();

#define ARG_SIZE 64

int hcix_get_args(char *args, int index, char *dest, size_t size)
{
    while(*args)
    {
        if(char_is_space(*args) || char_is_new_line(*args))
        {
            args++;
        }
        else
        {
            for(size_t i = 0; i < size; i++)
            {
                dest[i] = 0;
                if(!*args || char_is_space(*args) || char_is_new_line(*args)) break;
                dest[i] = *args++;
            }
            if(!index) return 1;
            index--;
        }
    }
    string_copy(dest, "", size);
    return 0;
}

void hcix_process_args(char *args)
{
    status_t st;
    int index = 0;
    char arg[ARG_SIZE];
    while(hcix_get_args(args, index, arg, ARG_SIZE))
    {
        if(string_equal(arg, "-console", ARG_SIZE))
        {
            index++;
            if(hcix_get_args(args, index, arg, ARG_SIZE))
            {
                st = device_find_by_name(arg);
                if(OK(st))
                {
                    console_set_phys(st.pointer);
                }
            }
        }
        else if(string_equal(arg, "-memory", ARG_SIZE))
        {
            index++;
            if(hcix_get_args(args, index, arg, ARG_SIZE))
            {
                // get value in MiB - 1024 bytes(low memory) - 256 bytes (reserved in some hardware)
                device_set_conf_by_name("memmgr", MEMMGR_CONF_LOWMEM_CAPACITY, 640);
                device_set_conf_by_name("memmgr", MEMMGR_CONF_HIGHMEM_CAPACITY, string_to_int32(arg, ARG_SIZE) * 1024 - 1280);
            }
        }
        index++;
    }
}

void hcix_init(char *args, size_t low_mem_kib, size_t high_mem_kib)
{
    status_t st;
    // initialize controllers
    ADD_MODULE(device);
    ADD_MODULE(memmgr);
    ADD_MODULE(cothread);
    ADD_MODULE(pcvideotext);
    // process kernel arguments
    hcix_process_args(args);
    // if memory capacity isn't set by args, set with bootloader info
    device_set_conf_by_name("memmgr", MEMMGR_CONF_LOWMEM_CAPACITY, low_mem_kib);
    device_set_conf_by_name("memmgr", MEMMGR_CONF_HIGHMEM_CAPACITY, high_mem_kib);
    // print header
    console_write_const("HCIX Operating System\n");
    console_write_const("=====================\n\n");
    // initialize memory manager
    device_init_by_name("memmgr");
    // print memory size
    st = device_get_conf_by_name("memmgr", MEMMGR_CONF_FREE_PAGES);
    console_write_const("Usable Memory: ");
    console_write_uint32(GET_VALUE(st, 0) * 4);
    console_write_const(" KiB / ");
    st = device_get_conf_by_name("memmgr", MEMMGR_CONF_TOTAL_PAGES);
    console_write_uint32(GET_VALUE(st, 0) * 4);
    console_write_const(" KiB\n");
    // execute threads
    while(1)
    {
        // cothread_idle();
        yield();
    }
}
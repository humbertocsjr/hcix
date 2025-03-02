#include "patuafs.h"

static FILE *_disk;

void disk_open(char *name)
{
    _disk = fopen(name, "rb+");
    if(!_disk)
    {
        fprintf(stderr, "error: can't open disk: %s\n", name);
        exit(1);
    }
}

int disk_read(uint32_t address, void *block)
{
    fseek(_disk, (long)address * 1024, SEEK_SET);
    return fread(block, 1024, 1, _disk);
}

int disk_write(uint32_t address, void *block)
{
    fseek(_disk, (long)address * 1024, SEEK_SET);
    return fwrite(block, 1024, 1, _disk);
}
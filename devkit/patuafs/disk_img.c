#include "patuafs.h"

static FILE *_disk;

int disk_open(patuafs_mount_t *mnt, char *name)
{
    mnt->disk = fopen(name, "rb+");
    if(!mnt->disk)
    {
        fprintf(stderr, "error: can't open disk: %s\n", name);
        exit(1);
    }
    return 1;
}

int disk_read(patuafs_mount_t *mnt, uint32_t address, void *block)
{
    fseek(mnt->disk, (long)address * 1024, SEEK_SET);
    return fread(block, 1024, 1, mnt->disk);
}

int disk_write(patuafs_mount_t *mnt, uint32_t address, void *block)
{
    fseek(mnt->disk, (long)address * 1024, SEEK_SET);
    return fwrite(block, 1024, 1, mnt->disk);
}
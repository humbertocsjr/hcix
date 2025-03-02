#include "patuafs.h"

int patuafs_mount(patuafs_mount_t *mnt, char *dev_name)
{
    if(!disk_open(mnt, dev_name)) return 0;

    return 1;
}
#include "patuafs.h"

int patuafs_mount(patuafs_mount_t *mnt, char *dev_name)
{
    if(!disk_open(mnt, dev_name)) return 0;

    return 1;
}

int patuafs_mkfs(patuafs_mount_t *mnt, char *dev_name, uint32_t total_blocks)
{
    char block[1024];
    if(!disk_open(mnt, dev_name)) return 0;
    // clean index
    memset(&mnt->index, 0, sizeof(patuafs_index_t));

    // write index
    memcpy(block, &mnt->index, sizeof(patuafs_index_t));
    disk_write(mnt, PATUAFS_INDEX_ADDRESS, block);

    return 1;
}
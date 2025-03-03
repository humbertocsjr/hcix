#include "patuafs.h"

int tree_read_node(patuafs_mount_t *mnt, patuafs_alloc_node_t *node, uint32_t node_number)
{
    patuafs_alloc_list_t list;
    memset(node, 0, sizeof(patuafs_alloc_node_t));
    if(node_number >= mnt->index.total_blocks) return 0;
    if(!disk_read(mnt, mnt->index.block_tree_address + (node_number >> 5), &list)) return 0;
    memcpy(node, &list.nodes[node_number & 31], sizeof(patuafs_alloc_node_t));
    return 1;
}

int tree_write_node(patuafs_mount_t *mnt, patuafs_alloc_node_t *node, uint32_t node_number)
{
    patuafs_alloc_list_t list;
    if(node_number >= mnt->index.total_blocks) return 0;
    if(!disk_read(mnt, mnt->index.block_tree_address + (node_number >> 5), &list)) return 0;
    memcpy(&list.nodes[node_number & 31], node, sizeof(patuafs_alloc_node_t));
    if(!disk_write(mnt, mnt->index.block_tree_address + (node_number >> 5), &list)) return 0;
    return 1;
}
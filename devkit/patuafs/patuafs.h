// Patuã File System
#ifndef  PATUAFS_H
#define  PATUAFS_H
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>

typedef struct patuafs_index_t
{
    uint32_t signature;
    uint32_t fs_version;
    uint32_t backup_version;
    uint32_t inode_version;
    uint32_t total_blocks;
    uint32_t total_inodes;
    uint32_t block_map_address;
    uint16_t block_map_size;
    uint32_t block_tree_address;
    uint32_t block_tree_size;
    uint32_t inodes_alloc_node;
    uint32_t inodes_map_alloc_node;
    uint32_t root_alloc_node;
    uint32_t backup_address;
} patuafs_index_t;

typedef struct patuafs_backup_t
{
    uint32_t counter;
    uint32_t address;
    uint32_t backup_address;
    uint32_t status;
} patuafs_backup_t;

typedef struct patuafs_bitmap_t
{
    uint8_t values[1024];
} patuafs_bitmap_t;

typedef struct patuafs_datetime_t
{
    int8_t year; // 2000 + year
    uint8_t month;
    uint8_t day;
    uint8_t hour;
    uint8_t minute;
    uint8_t second;
} patuafs_datetime_t;

typedef struct patuafs_inode_t
{
    uint16_t mode;
    char name[96];
    uint16_t user_id;
    uint16_t group_id;
    uint16_t links;
    patuafs_datetime_t creation;
    patuafs_datetime_t modification;
    uint32_t size;
    uint32_t data_alloc_node;
    uint32_t metadata_alloc_node;
} patuafs_inode_t;

typedef struct patuafs_inode_list_t
{
    patuafs_inode_t inodes[8];
} patuafs_inode_list_t;

#define PATUAFS_MASK_BLOCK_NUMBER 0x00ffffff
#define PATUAFS_MASK_HIGH 0xff000000
#define PATUAFS_GET_BLOCK_NUMBER(blk_num) (blk_num & PATUAFS_GET_BLOCK_NUMBER)
#define PATUAFS_GET_HEIGHT(blk_num) ((blk_num & PATUAFS_MASK_HIGH)>>24)

typedef struct patuafs_alloc_node_t
{
    uint32_t block_number;
    uint32_t left;
    uint32_t right;
    uint32_t data_address;
} patuafs_alloc_node_t;

typedef struct patuafs_alloc_list_t
{
    patuafs_alloc_node_t nodes[32];
} patuafs_alloc_list_t;

typedef FILE patuafs_disk_t;

typedef struct patuafs_mount_t
{
    patuafs_index_t index;
    patuafs_disk_t *disk;
} patuafs_mount_t;

#define PATUAFS_INDEX_ADDRESS 16

int disk_open(patuafs_mount_t *mnt, char *name);
int disk_read(patuafs_mount_t *mnt, uint32_t address, void *block);
int disk_write(patuafs_mount_t *mnt, uint32_t address, void *block);

#endif
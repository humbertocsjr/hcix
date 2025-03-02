#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <stdint.h>

#pragma pack(1)

typedef struct boot_header_t
{
    char signature[2];
    uint16_t kernel_blocks;
    uint16_t sectors;
    uint16_t heads;
    uint16_t cylinders;
} boot_header_t;

FILE *open_file(char *fname, char *mode)
{
    FILE *file = fopen(fname, mode);
    if(file == 0)
    {
        fprintf(stderr, "error: can't open file: %s\n", fname);
        exit(1);
    }
    return file;
}

int main(int argc, char **argv)
{
    FILE *img;
    FILE *bootsect;
    FILE *kernel;
    char block[512];
    size_t size;

    if(argc != 7)
    {
        fprintf(stderr, "Usage: installboot [disk/image] [kernel] [bootsector] [cylinders] [heads] [sectors]\n");
        return 1;
    }
    img = open_file(argv[1], "rb+");
    kernel = open_file(argv[2], "rb");
    bootsect = open_file(argv[3], "rb");
    fread(block, 1, 512, bootsect);
    boot_header_t *header = (boot_header_t *)&block[290];
    if(header->signature[0] != 'H' || header->signature[1] != 'C')
    {
        fprintf(stderr, "error: invalid boot sector.\n");
        return 1;
    }
    // calculate boot sector
    header->cylinders = atoi(argv[4]);
    header->heads = atoi(argv[5]);
    header->sectors = atoi(argv[6]);
    fseek(kernel, 0, SEEK_END);
    size = (ftell(kernel) + 511) / 512;
    fseek(kernel, 0, SEEK_SET);
    header->kernel_blocks = size;
    // write boot sector
    fseek(img, 0, SEEK_SET);
    fwrite(block, 1, 512, img);
    // write kernel
    while(fread(block, 1, 512, kernel) > 0)
    {
        fwrite(block, 1, 512, img);
    }
    fclose(img);
    fclose(bootsect);
    fclose(kernel);

    return 0;
}
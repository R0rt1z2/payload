#pragma once

#include <stdint.h>

#define BLOCK_SIZE 512

typedef struct __part part_t;
typedef struct __part_dev part_dev_t;
typedef struct __block_dev_desc block_dev_desc_t;

struct __block_dev_desc {
    int		        dev;
    unsigned long   lba;
    unsigned long	blksz;
    unsigned long	(*block_read)(int dev,
                                   unsigned long start,
                                   unsigned long blkcnt,
                                   void *buffer,
                                   unsigned int part_id);
    unsigned long	(*block_write)(int dev,
                                    unsigned long start,
                                    unsigned long blkcnt,
                                    const void *buffer,
                                    unsigned int part_id);
};

struct part_meta_info {
    uint8_t name[64];
    uint8_t uuid[16];
};

struct __part
{
    unsigned long  start_sect;
    unsigned long  nr_sects;
    unsigned int part_id;
    char *name;
    struct part_meta_info *info;
};

struct __part_dev
{
    int init;
    int id;
    block_dev_desc_t *blkdev;
    int (*init_dev) (int id);
    int (*read)  (part_dev_t *dev, uint64_t src, unsigned char *dst, int size, unsigned int part_id);
    int (*write) (part_dev_t *dev, unsigned char *src, uint64_t dst, int size, unsigned int part_id);
};

part_dev_t *mt_part_get_device(void);
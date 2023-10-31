#include "include/part.h"

#include <stdint.h>

uint64_t emmc_write(uint32_t part_id, uint64_t offset, void *data, uint64_t size)
{
    part_dev_t *dev = mt_part_get_device();
    return (uint64_t)dev->write(dev, data, offset, (int)size, part_id);
}

uint64_t emmc_read(uint32_t part_id, uint64_t lba, void *data, uint64_t size)
{
    part_dev_t *dev = mt_part_get_device();
    return (uint64_t)dev->read(dev, lba * BLOCK_SIZE, data, (int)size, part_id);
}
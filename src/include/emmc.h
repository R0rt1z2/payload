#pragma once

#include <stdint.h>

uint64_t emmc_write(uint32_t part_id, uint64_t offset, void *data, uint64_t size);
uint64_t emmc_read(uint32_t part_id, uint64_t lba, void *data, uint64_t size);
#include "include/efi.h"
#include "include/emmc.h"
#include "include/printf.h"
#include "include/part.h"
#include "include/memory.h"
#include "include/string.h"

gpt_entry_t *gpt_entries = NULL;
uint32_t partition_count = 0;

gpt_entry_t *parse_gpt() {
    part_dev_t *dev = mt_part_get_device();
    if (dev == NULL || dev->blkdev == NULL) {
        LOGE("Failed to get block device\n");
        return NULL;
    }

    uint8_t *buf = malloc(512);
    int ret = emmc_read(0, 1, buf, 512);
    if (ret < 0) {
        LOGE("Failed to read GPT header\n");
        free(buf);
        return NULL;
    }

    gpt_header_t gpt_header;
    memcpy(&gpt_header, buf, sizeof(gpt_header));
    if (gpt_header.signature != GPT_HEADER_SIGNATURE) {
        LOGE("Invalid GPT signature (0x%llx vs 0x%llx)\n",
                     gpt_header.signature, GPT_HEADER_SIGNATURE);
        free(buf);
        return NULL;
    }

    partition_count = gpt_header.number_of_partition_entries;
    gpt_entries = malloc(sizeof(gpt_entry_t) * partition_count);
    if (gpt_entries == NULL) {
        LOGE("Failed to allocate memory for GPT entries\n");
        free(buf);
        return NULL;
    }

    uint32_t entries_per_block = BLOCK_SIZE / gpt_header.size_of_partition_entry;
    uint32_t partition_index = 0;

    for (uint32_t i = 0; i < gpt_header.number_of_partition_entries; i += entries_per_block) {
        uint32_t lba = gpt_header.partition_entry_lba + i * gpt_header.size_of_partition_entry / BLOCK_SIZE;
        ret = emmc_read(0, lba, buf, BLOCK_SIZE);

        for (uint32_t j = 0; j < entries_per_block && partition_index < partition_count; j++) {
            gpt_entry_t *entry = (gpt_entry_t *)(buf + j * gpt_header.size_of_partition_entry);

            for (int k = 0; k < 16; k++) {
                gpt_entries[partition_index].partition_type_guid[k] = entry->partition_type_guid[k];
            }

            for (int k = 0; k < 16; k++) {
                gpt_entries[partition_index].unique_partition_guid[k] = entry->unique_partition_guid[k];
            }

            gpt_entries[partition_index].starting_lba = entry->starting_lba;
            gpt_entries[partition_index].ending_lba = entry->ending_lba;
            gpt_entries[partition_index].attributes = entry->attributes;

            for (int k = 0; k < 36; k++) {
                gpt_entries[partition_index].partition_name[k] = entry->partition_name[k];
            }

            partition_index++;
        }
    }

    free(buf);
    return gpt_entries;
}

int get_partition_count() {
    return partition_count;
}

int read_partition(const char *name, void *buffer, uint64_t len) {
    if (gpt_entries == NULL || name == NULL || buffer == NULL) {
        LOGE("Invalid input or GPT entries not parsed\n");
        return -1;
    }

    for (uint32_t i = 0; i < partition_count; i++) {
        if (wstrcmp(gpt_entries[i].partition_name, name) == 0) {
            uint64_t partition_size_in_bytes = (gpt_entries[i].ending_lba - gpt_entries[i].starting_lba + 1) * BLOCK_SIZE;
            uint64_t read_size = len > partition_size_in_bytes ? partition_size_in_bytes : len;

            uint64_t read_blocks = read_size / BLOCK_SIZE;
            uint64_t remaining_bytes = read_size % BLOCK_SIZE;
            uint64_t read_offset = gpt_entries[i].starting_lba * BLOCK_SIZE;
            uint8_t *read_buffer = (uint8_t *)buffer;

            while (read_blocks--) {
                if (emmc_read(0, read_offset / BLOCK_SIZE, read_buffer, BLOCK_SIZE) != BLOCK_SIZE) {
                    LOGE("Failed to read partition: %s at LBA: %08x\n", name, read_offset / BLOCK_SIZE);
                    return -1;
                }
                read_buffer += BLOCK_SIZE;
                read_offset += BLOCK_SIZE;
            }

            if (remaining_bytes > 0) {
                uint8_t buf[BLOCK_SIZE];
                if (emmc_read(0, read_offset / BLOCK_SIZE, buf, BLOCK_SIZE) != BLOCK_SIZE) {
                    LOGE("Failed to read last block of partition: %s\n", name);
                    return -1;
                }
                memcpy(read_buffer, buf, remaining_bytes);
            }

            return 0;
        }
    }

    LOGE("Unable to find partition: %s\n", name);
    return -1;
}

int write_partition(const char *name, void *buffer, uint64_t len) {
    if (gpt_entries == NULL || name == NULL || buffer == NULL) {
        LOGE("Invalid input or GPT entries not parsed yet\n");
        return -1;
    }

    for (uint32_t i = 0; i < partition_count; i++) {
        if (wstrcmp(gpt_entries[i].partition_name, name) == 0) {
            uint64_t partition_size_in_bytes = (gpt_entries[i].ending_lba - gpt_entries[i].starting_lba + 1) * BLOCK_SIZE;
            uint64_t write_size = len > partition_size_in_bytes ? partition_size_in_bytes : len;

            uint64_t written_blocks = write_size / BLOCK_SIZE;
            uint64_t remaining_bytes = write_size % BLOCK_SIZE;
            uint64_t write_offset = gpt_entries[i].starting_lba * BLOCK_SIZE;
            uint8_t *write_buffer = (uint8_t *)buffer;

            while (written_blocks--) {
                if (emmc_write(0, write_offset / BLOCK_SIZE, write_buffer, BLOCK_SIZE) != BLOCK_SIZE) {
                    LOGE("Failed to write partition: %s at LBA: %08x\n", name, write_offset / BLOCK_SIZE);
                    return -1;
                }
                write_buffer += BLOCK_SIZE;
                write_offset += BLOCK_SIZE;
            }

            if (remaining_bytes > 0) {
                uint8_t buf[BLOCK_SIZE];
                if (emmc_read(0, write_offset / BLOCK_SIZE, buf, BLOCK_SIZE) != BLOCK_SIZE) {
                    LOGE("Failed to prepare last block of partition: %s\n", name);
                    return -1;
                }

                memcpy(buf, write_buffer, remaining_bytes);
                if (emmc_write(0, write_offset / BLOCK_SIZE, buf, BLOCK_SIZE) != BLOCK_SIZE) {
                    LOGE("Failed to write last block of partition: %s\n", name);
                    return -1;
                }
            }

            return 0;
        }
    }

    LOGE("Unable to find partition: %s\n", name);
    return -1;
}

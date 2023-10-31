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
        video_printf("Failed to get block device\n");
        return NULL;
    }

    uint8_t *buf = malloc(512);
    int ret = emmc_read(0, 1, buf, 512);
    if (ret < 0) {
        video_printf("Failed to read GPT header\n");
        free(buf);
        return NULL;
    }

    gpt_header_t gpt_header;
    memcpy(&gpt_header, buf, sizeof(gpt_header));
    if (gpt_header.signature != GPT_HEADER_SIGNATURE) {
        video_printf("Invalid GPT signature (0x%llx vs 0x%llx)\n",
                     gpt_header.signature, GPT_HEADER_SIGNATURE);
        free(buf);
        return NULL;
    }

    partition_count = gpt_header.number_of_partition_entries;
    gpt_entries = malloc(sizeof(gpt_entry_t) * partition_count);
    if (gpt_entries == NULL) {
        video_printf("Failed to allocate memory for GPT entries\n");
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
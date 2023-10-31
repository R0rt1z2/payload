#include "include/printf.h"
#include "include/common.h"
#include "include/string.h"
#include "include/efi.h"

__attribute__((section(".text.start"))) int main(void) {
    // Perform basic initialization
    kaeru_init();

    // Parse and read GPT
    gpt_entry_t *gpt_entries = parse_gpt();
#if DEBUG
    for (uint32_t i = 0; i < get_partition_count(); i++) {
        gpt_entry_t *entry = &gpt_entries[i];
        char *name = w2ascii(entry->partition_name);
        if (entry->starting_lba != 0 && name != NULL && name[0] != 0) {
            video_printf("Partition %d: %s\n", i, name);
        }
        free(name);
    }
#endif

    // Jump to original app()
    int ret = app();

    // The caller should never return
    video_printf("Oops, original app() returned %d!\n", ret);
    while (1) {
        // Wait for interrupts
        __asm__("wfi");
    }
}
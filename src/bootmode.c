#include "include/bootmode.h"
#include "include/printf.h"

#include <stdint.h>

bootmode_t get_bootmode() {
    return *(volatile uint32_t *)0x48172bbc;
}

void force_bootmode(bootmode_t mode) {
    dprintf("Forcing %d boot mode...\n", mode);
    *(volatile uint16_t *)0x48021c0e = 0x200 | (mode & 0xff);
    arch_clean_invalidate_cache_range(0x48021c0e, 2);
}

void show_bootmode(bootmode_t mode) {
    switch (mode) {
        case BOOTMODE_NORMAL:
            video_printf("=> NORMAL mode...\n");
            break;
        case BOOTMODE_META:
            video_printf("=> META mode...\n");
            break;
        case BOOTMODE_RECOVERY:
            video_printf("=> RECOVERY mode...\n");
            break;
        case BOOTMODE_FACTORY:
            video_printf("=> FACTORY mode...\n");
            break;
        case BOOTMODE_ADVMETA:
            video_printf("=> ADVMETA mode...\n");
            break;
        case BOOTMODE_ATEFACT:
            video_printf("=> ATEFACT mode...\n");
            break;
        case BOOTMODE_ALARM:
            video_printf("=> ALARM mode...\n");
            break;
        case BOOTMODE_FASTBOOT:
            video_printf("=> FASTBOOT mode...\n");
            break;
        case BOOTMODE_ERECOVERY:
            video_printf("=> ERECOVERY mode...\n");
            break;
        default:
            video_printf("=> UNKNOWN mode...\n");
            break;
    }
}

void handle_late_bootmode() {
    bootmode_t mode = get_bootmode();
    if (mode == BOOTMODE_ADVMETA) {
        // Use advanced factory boot to boot into eRecovery since
        // there's no button combo for it.
        mode = BOOTMODE_ERECOVERY;
    }

    if (mode == BOOTMODE_ERECOVERY || mode == BOOTMODE_RECOVERY) {
        // This is mostly used to be able to distinguish between
        // recovery and eRecovery mode (thanks Huawei).
        show_bootmode(mode);
    }
}

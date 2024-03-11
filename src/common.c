#include "include/arm.h"
#include "include/bootmode.h"
#include "include/commands.h"
#include "include/common.h"
#include "include/printf.h"

#include <stdbool.h>

void platform_init() {
    ((void (*)(void))(0x48007234 | 1))();
}

int app() {
    return ((int (*)(void))(0x4802ccd8 | 1))();
}

bool mtk_detect_key(unsigned short key) {
    return ((bool (*)(unsigned short))(0x48008e20 | 1))(key);
}

void patch_lk_early() {
    ((volatile uint32_t *)0x4817fb58)[0] = 1; // userlock state: UNLOCKED
    arch_clean_invalidate_cache_range(0x4817fb58, 4);

    ((volatile uint32_t *)0x4817fb54)[0] = 1; // fblock state: UNLOCKED
    arch_clean_invalidate_cache_range(0x4817fb54, 4);

    ((volatile uint32_t *)0x4811f294)[0] = 0; // device state: GREEN
    arch_clean_invalidate_cache_range(0x4811f294, 4);

    *(uint32_t *)0x4817fb4c = 1; // widevine state: UNLOCKED
    arch_clean_invalidate_cache_range(0x4817fb4c, 4);
}

void patch_lk_late() {
    volatile uint16_t *x = (volatile uint16_t *)0x48031de0; // lock_status_check
    x[0] = 0x4770; // bx lr
    arch_clean_invalidate_cache_range((uint32_t)x, 2);

    x = (volatile uint16_t *)0x4805abd0; // show_boot_warning
    x[0] = 0x4770; // bx lr
    arch_clean_invalidate_cache_range((uint32_t)x, 2);

    x = (volatile uint16_t *)0x480348d4; // power_check
    x[0] = 0x2001; // movs r0, #1
    x[1] = 0x4770; // bx lr
    arch_clean_invalidate_cache_range((uint32_t)x, 4);

    x = (volatile uint16_t *)0x48031d38; // read_root_status
    x[0] = 0x2001; // movs r0, #1
    x[1] = 0x4770; // bx lr
    arch_clean_invalidate_cache_range((uint32_t)x, 4);

    x = (volatile uint16_t *)0x48032274; // get_userlock_state
    x[0] = 0x2001; // movs r0, #1
    x[1] = 0x4770; // bx lr
    arch_clean_invalidate_cache_range((uint32_t)x, 4);

    x = (volatile uint16_t *)0x48032388; // get_fblock_state
    x[0] = 0x2001; // movs r0, #1
    x[1] = 0x4770; // bx lr
    arch_clean_invalidate_cache_range((uint32_t)x, 4);
}

void kaeru_early_init() {
    __asm__("mcr p15, 0, %0, c7, c5, 0" : : "r" (0));
    patch_lk_early();
    dprintf("Early initialization complete!\n");
}

void kaeru_late_init() {
    patch_lk_late();
    handle_late_bootmode();
    LOGD("Late initialization complete!\n");
}
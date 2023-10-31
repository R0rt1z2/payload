#include "include/arm.h"
#include "include/bootmode.h"
#include "include/commands.h"
#include "include/common.h"
#include "include/printf.h"

#include <stdbool.h>

void platform_init() {
    ((void (*)(void))(0x48007a80 | 1))();
}

int app() {
    return ((int (*)(void))(0x48028c78 | 1))();
}

bool mtk_detect_key(unsigned short key) {
    return ((bool (*)(unsigned short))(0x4800942c | 1))(key);
}

void patch_lk_early() {
    ((volatile uint32_t *)0x481773a8)[0] = 1; // userlock state: UNLOCKED
    arch_clean_invalidate_cache_range(0x481773a8, 4);

    ((volatile uint32_t *)0x481773a4)[0] = 1; // fblock state: UNLOCKED
    arch_clean_invalidate_cache_range(0x481773a4, 4);

    ((volatile uint32_t *)0x48119788)[0] = 0; // device state: GREEN
    arch_clean_invalidate_cache_range(0x48119788, 4);

    *(uint32_t *)0x4817739c = 1; // widevine state: UNLOCKED
    arch_clean_invalidate_cache_range(0x4817739c, 4);
}

void patch_lk_late() {
    volatile uint16_t *x = (volatile uint16_t *)0x4802dedc; // lock_status_check
    x[0] = 0x4770; // bx lr
    arch_clean_invalidate_cache_range((uint32_t)x, 2);

    x = (volatile uint16_t *)0x480566a8; // show_boot_warning
    x[0] = 0x4770; // bx lr
    arch_clean_invalidate_cache_range((uint32_t)x, 2);

    x = (volatile uint16_t *)0x48029ff8; // fastboot_oem_wrapper
    x[0] = 0x46c0; // nop
    x[1] = 0x46c0; // nop
    arch_clean_invalidate_cache_range((uint32_t)x, 4);

    x = (volatile uint16_t *)0x480309c4; // power_check
    x[0] = 0x2001; // movs r0, #1
    x[1] = 0x4770; // bx lr
    arch_clean_invalidate_cache_range((uint32_t)x, 4);

    x = (volatile uint16_t *)0x4802de34; // read_root_status
    x[0] = 0x2001; // movs r0, #1
    x[1] = 0x4770; // bx lr
    arch_clean_invalidate_cache_range((uint32_t)x, 4);

    x = (volatile uint16_t *)0x4802e370; // get_userlock_state
    x[0] = 0x2001; // movs r0, #1
    x[1] = 0x4770; // bx lr
    arch_clean_invalidate_cache_range((uint32_t)x, 4);

    x = (volatile uint16_t *)0x4802e484; // get_fblock_state
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
    register_commands();
    LOGD("Late initialization complete!\n");
}
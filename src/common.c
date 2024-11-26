#include "include/arm.h"
#include "include/bootmode.h"
#include "include/commands.h"
#include "include/common.h"
#include "include/printf.h"

#include <stdbool.h>

void platform_init() {
    ((void (*)(void))(0x4800f57c | 1))();
}

int app() {
    return ((int (*)(void))(0x480385f8 | 1))();
}

bool mtk_detect_key(unsigned short key) {
    return ((bool (*)(unsigned short))(0x480114d4 | 1))(key);
}

void patch_lk_early() {
    ((volatile uint32_t *)0x48257110)[0] = 1; // userlock state: UNLOCKED
    arch_clean_invalidate_cache_range(0x48257110, 4);

    ((volatile uint32_t *)0x4817b23c)[0] = 0; // device state: GREEN
    arch_clean_invalidate_cache_range(0x4817b23c, 4);
}

void patch_lk_late() {
    volatile uint16_t *x = (volatile uint16_t *)0x4808c2f8; // show_boot_warning
    x[0] = 0x4770; // bx lr
    arch_clean_invalidate_cache_range((uint32_t)x, 2);

    x = (volatile uint16_t *)0x480391f8; // fastboot_oem_wrapper
    x[0] = 0x46c0; // nop
    x[1] = 0x46c0; // nop
    arch_clean_invalidate_cache_range((uint32_t)x, 4);

    x = (volatile uint16_t *)0x4803fa7c; // power_check
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
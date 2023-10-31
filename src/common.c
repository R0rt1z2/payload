#include "include/common.h"
#include "include/printf.h"
#include "include/commands.h"

int app() {
    return ((int (*)(void))(0x41e1ccc8 | 1))();
}

void arch_clean_invalidate_cache_range(uint32_t start, uint32_t size) {
    return ((void (*)(uint32_t, uint32_t))0x41e1a528)(start, size);
}

void patch_lk() {
    volatile uint16_t *x = (volatile uint16_t *)0x41e289b8; // sec_dl_permission_chk
    x[1] = 0x2000; // movs r0, #0
    x[2] = 0x4770; // bx lr

    x = (volatile uint16_t *)0x41e1ed84; // power_check
    x[0] = 0x2001; // movs r0, #1
    x[1] = 0x4770; // bx lr

    x = (volatile uint16_t *)0x41e2716c; // seclib_sec_boot_enabled
    x[0] = 0x2000; // movs r0, #0
    x[1] = 0x4770; // bx lr

    x = (volatile uint16_t *)0x41e29058; // sec_usbdl_enabled
    x[0] = 0x2000; // movs r0, #0
    x[1] = 0x4770; // bx lr

    ((volatile uint32_t *)0x41e44518)[0] = 0; // device state: GREEN
}

void kaeru_init() {
#ifdef DEBUG
    video_printf("Invalidating caches...");
#endif
    __asm__("mcr p15, 0, %0, c7, c5, 0" : : "r" (0));
#ifdef DEBUG
    video_printf(" OK\n");
    video_printf("Applying LK memory patches...");
#endif
    patch_lk();
#ifdef DEBUG
    video_printf(" OK\n");
    video_printf("Registering fastboot commands...");
#endif
    register_commands();
#ifdef DEBUG
    video_printf(" OK\n");
#endif
}
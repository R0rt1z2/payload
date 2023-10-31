#include "include/arm.h"
#include "include/bootmode.h"
#include "include/common.h"
#include "include/efi.h"
#include "include/printf.h"

void kaeru() {
    // Perform late initialization
    kaeru_late_init();

    // Parse GPT
    gpt_entry_t *gpt_entries;
    gpt_entries = parse_gpt();

    // Jump to original app()
    int ret = app();

    // The caller should never return
    LOGE("app() returned: %d\n", ret);
    while (1) {
        // Wait for interrupts
        __asm__("wfi");
    }
}

__attribute__((section(".text.start"))) int main(void) {
    // Patch the blx call to app() so we jump to kaeru
    // instead. This is done here because platform_init
    // gets called early in the boot process.
    patch_call(APP_CALLER, kaeru);

    // Huawei messed up the boot mode detection, so we
    // need to force recovery mode if the user holds
    // the volume up button during boot.
    if (mtk_detect_key(8)) {
        force_bootmode(BOOTMODE_RECOVERY);
    }

    // Perform early initialization required for kaeru
    kaeru_early_init();

    // Naturally, we need to call the original function
    // since we want our hardware to be initialized.
    platform_init();

    // We should actually return here, so bootstrap2 can
    // continue its boot flow.
    return 0; // WDT can't kick in if we return.
}

#pragma once

#include <stdbool.h>

#define LK_BASE 0x48000000
#define STACK_END 0x4825d31c
#define PAYLOAD_DST STACK_END + 0x7C // 0x4825D398

#define APP_CALLER 0x48030644
#define PLATFORM_INIT_CALLER 0x4802f708

int app();
void platform_init();
bool mtk_detect_key(unsigned short key);
void kaeru_late_init();
void kaeru_early_init();

#pragma once

#include <stdbool.h>

#define LK_BASE 0x48000000
#define STACK_END 0x4819BC9C
#define PAYLOAD_DST STACK_END + 0x7C // 0x4819BD18

#define APP_CALLER 0x48028BE8
#define PLATFORM_INIT_CALLER 0x48027CB0

int app();
void platform_init();
bool mtk_detect_key(unsigned short key);
void kaeru_late_init();
void kaeru_early_init();

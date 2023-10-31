#pragma once

#include <stdint.h>

#define LK_BASE 0x41e00000
#define PAYLOAD_DST 0x41e58648
#define PAYLOAD_CALLER 0x41e1b9b4

int app();
void kaeru_init();
void arch_clean_invalidate_cache_range(uint32_t start, uint32_t size);
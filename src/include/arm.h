#pragma once

#include <stdint.h>

unsigned int __aeabi_uidiv(unsigned int n, unsigned int d);
void patch_call(uint32_t patch_addr, void (*target_func)(void));
void arch_clean_invalidate_cache_range(uint32_t start, uint32_t size);
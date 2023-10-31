#include "include/arm.h"
#include "include/printf.h"

// ARM does not have a division instruction.
unsigned int __aeabi_uidiv(unsigned int n, unsigned int d) {
    unsigned int q = 0;
    while (n >= d) {
        n -= d;
        q++;
    }
    return q;
}

void patch_call(uint32_t patch_addr, void (*target_func)(void)) {
    uint32_t current_address = patch_addr + 4; // In Thumb, PC is ahead by 4
    uint32_t target_address = (uint32_t)target_func & ~1; // (LSB cleared for calculation)
    int32_t offset = target_address - current_address;

    uint16_t high_offset = (offset >> 12) & 0x7FF;
    uint16_t low_offset = (offset >> 1) & 0x7FF;

    uint16_t high_instruction = 0xF000 | high_offset;
    uint16_t low_instruction = 0xF800 | low_offset;

    volatile uint16_t* ptr = (volatile uint16_t*)patch_addr;
    *ptr = high_instruction;
    *(ptr + 1) = low_instruction;
}

void arch_clean_invalidate_cache_range(uint32_t start, uint32_t size) {
    return ((void (*)(uint32_t, uint32_t))0x48020ea0)(start, size);
}
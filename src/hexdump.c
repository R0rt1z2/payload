#include "include/printf.h"
#include "include/hexdump.h"

#include <stddef.h>

void uart_hexdump(const void *data, size_t size) {
    size_t i;
    size_t j;

    for (i = 0; i < size; i += 16) {
        uart_printf("%08x: ", i);
        for (j = 0; j < 16; j++) {
            if (i + j < size) {
                uart_printf("%02x ", ((uint8_t *)data)[i + j]);
            } else {
                uart_printf("   ");
            }
        }

        uart_printf(" ");
        for (j = 0; j < 16; j++) {
            if (i + j < size) {
                uint8_t c = ((uint8_t *)data)[i + j];
                uart_printf("%c", (c >= 32 && c < 127) ? c : '.');
            }
        }
        uart_printf("\n");
    }
}

void video_hexdump(const void *data, size_t size) {
    size_t i;
    size_t j;

    for (i = 0; i < size; i += 16) {
        video_printf("%08x: ", i);
        for (j = 0; j < 16; j++) {
            if (i + j < size) {
                video_printf("%02x ", ((uint8_t *)data)[i + j]);
            } else {
                video_printf("   ");
            }
        }

        video_printf(" ");
        for (j = 0; j < 16; j++) {
            if (i + j < size) {
                uint8_t c = ((uint8_t *)data)[i + j];
                video_printf("%c", (c >= 32 && c < 127) ? c : '.');
            }
        }
        video_printf("\n");
    }
}
#pragma once

#include <stdint.h>
#include <stdlib.h>

#define UART_REG0 0x11002014
#define UART_REG1 0x11002000

size_t video_printf(const char *format, ...);
size_t uart_printf(const char *format, ...);
void uart_hexdump(const void *data, size_t size);
void video_hexdump(const void *data, size_t size);
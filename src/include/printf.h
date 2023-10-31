#pragma once

#include <stdint.h>
#include <stdlib.h>

#define UART_REG0 0x11002014
#define UART_REG1 0x11002000

size_t video_printf(const char *format, ...);
size_t uart_printf(const char *format, ...);
size_t dprintf(const char *format, ...);
void uart_hexdump(const void *data, size_t size);
void video_hexdump(const void *data, size_t size);

#define LOGI(...) \
    do { \
        video_printf("[INFO] " __VA_ARGS__); \
        dprintf("[INFO] " __VA_ARGS__);      \
    } while (0)

#define LOGE(...) \
    do { \
        video_printf("[ERROR] " __VA_ARGS__); \
        dprintf("[ERROR] " __VA_ARGS__);      \
    } while (0)

#define LOGW(...) \
    do { \
        video_printf("[WARNING] " __VA_ARGS__); \
        dprintf("[WARNING] " __VA_ARGS__);      \
    } while (0)

#if DEBUG == 1
#define LOGD(...) \
    do { \
        video_printf("[DEBUG] " __VA_ARGS__); \
        dprintf("[DEBUG] " __VA_ARGS__);      \
    } while (0)
#else
#define LOGD(...)
#endif
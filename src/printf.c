#include "include/printf.h"

#include <stdarg.h>

void low_uart_put(int ch) {
    if (ch == '\n') ch = '\r';
    while (*((volatile uint32_t *)UART_REG0) & 0x20);
    *((volatile uint32_t *)UART_REG1) = ch;
}

typedef struct uidiv_result {
    uint32_t quo;  /// < Quotient
    uint32_t rem;  /// < Remainder
} uidiv_result_t;

uidiv_result_t uidiv(uint32_t num, uint32_t dem) {
    uidiv_result_t result = {0, 0};

    if (dem == 0) {
        return result;
    }

    uint32_t temp = dem;

    while (temp <= num >> 1) {
        temp <<= 1;
    }

    while (temp >= dem) {
        if (num >= temp) {
            num -= temp;
            result.quo = (result.quo << 1) | 1;
        } else {
            result.quo <<= 1;
        }
        temp >>= 1;
    }

    result.rem = num;

    return result;
}

static void _putn(char **p_str, uint32_t x, uint32_t base, char fill, int fcnt, int upper) {
    char buf[65];
    char *digits = upper ? "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ" : "0123456789abcdefghijklmnopqrstuvwxyz";
    char *p = buf + 64;
    int c = fcnt;
    uidiv_result_t div_res;

    if (base > 36) return;

    *p = '\0';
    do {
        c--;
        div_res = uidiv(x, base);
        *--p = digits[div_res.rem];
        x = div_res.quo;
    } while (x);

    while (c-- > 0) *--p = fill;
    while (*p) *(*p_str)++ = *p++;
}

int vsprintf(char *str, const char *fmt, va_list ap) {
    char *s;
    char c, fill;
    int fcnt;
    uint32_t n;

    while (*fmt) {
        if (*fmt == '%') {
            fmt++;
            fill = 0;
            fcnt = 0;
            if ((*fmt >= '0' && *fmt <= '9') || *fmt == ' ') {
                if (*(fmt + 1) >= '0' && *(fmt + 1) <= '9') {
                    fill = *fmt;
                    fcnt = *(fmt + 1) - '0';
                    fmt += 2;
                }
            }
            switch (*fmt) {
                case 'c':
                    c = (char)va_arg(ap, int);
                    *str++ = c;
                    break;
                case 's':
                    s = va_arg(ap, char *);
                    while (*s) *str++ = *s++;
                    break;
                case 'u':
                    n = va_arg(ap, uint32_t);
                    _putn(&str, n, 10, fill, fcnt, 0);
                    break;
                case 'x':
                    n = va_arg(ap, uint32_t);
                    _putn(&str, n, 16, fill, fcnt, 0);
                    break;
                case 'X':
                    n = va_arg(ap, uint32_t);
                    _putn(&str, n, 16, fill, fcnt, 1);
                    break;
                case '%':
                    *str++ = '%';
                    break;
                case '\0':
                    goto out;
                default:
                    *str++ = '%';
                    *str++ = *fmt;
                    break;
            }
        } else {
            *str++ = *fmt;
        }
        fmt++;
    }

    out:
    *str = '\0';
    return 0;
}

int putchar(int c) {
    low_uart_put(c);
    return 0;
}

size_t uart_printf(const char *format, ...) {
    char line[512] = {0};
    va_list arg;

    va_start (arg, format);
    vsprintf (line, format, arg);
    va_end (arg);

    for (char *c = line; *c; ++c) {
        putchar(*c);
    }

    return 0;
}

size_t video_printf(const char *format, ...) {
    return ((size_t (*)(const char *format, ...))(0x480644f0 | 1))(format);
}

size_t dprintf(const char *format, ...) {
    return ((size_t (*)(const char *format, ...))(0x48064bc8 | 1))(format);
}
#include "include/fastboot.h"

void fastboot_info(const char *reason) {
    ((void (*)(const char *reason))(0x48029bfc | 1))(reason);
}

void fastboot_fail(const char *reason) {
    ((void (*)(const char *reason))(0x48029c58 | 1))(reason);
}

void fastboot_okay(const char *reason) {
    ((void (*)(const char *reason))(0x48029dd0 | 1))(reason);
}

void fastboot_register(const char *prefix,
                       void (*handle)(const char *arg, void *data, unsigned sz),
                       unsigned char security_enabled) {
    ((void (*)(const char *prefix, void (*handle)(const char *arg, void *data, unsigned sz), unsigned char security_enabled))(0x4802991c | 1))(prefix, handle, security_enabled);
}

void fastboot_enter() {
    ((void (*)(void))(0x480200c4 | 1))();
}
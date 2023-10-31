#pragma once

void fastboot_info(const char *reason);
void fastboot_fail(const char *reason);
void fastboot_okay(const char *reason);
void fastboot_register(const char *prefix,
                       void (*handle)(const char *arg, void *data, unsigned sz),
                       unsigned char security_enabled);
void fastboot_enter();
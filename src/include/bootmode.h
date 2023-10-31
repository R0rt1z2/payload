#pragma once

typedef enum {
    BOOTMODE_NORMAL = 0,
    BOOTMODE_META = 1,
    BOOTMODE_RECOVERY = 2,
    BOOTMODE_FACTORY = 4,
    BOOTMODE_ADVMETA = 5,
    BOOTMODE_ATEFACT = 6,
    BOOTMODE_ALARM = 7,
    BOOTMODE_FASTBOOT = 99,
    BOOTMODE_ERECOVERY = 101,
} bootmode_t;

bootmode_t get_bootmode();
void force_bootmode(bootmode_t mode);
void show_bootmode(bootmode_t mode);
void handle_late_bootmode();

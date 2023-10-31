#include "include/printf.h"
#include "include/string.h"
#include "include/fastboot.h"
#include "include/commands.h"

void cmd_hexdump(const char *arg, void *data, unsigned sz) {
    char *args = strdup(arg);
    char *a = strtok(args, " ");
    char *s = strtok(NULL, " ");

    if (a != NULL && s != NULL) {
        long al = strtol(a, NULL, 0);
        long sl = strtol(s, NULL, 0);
        video_hexdump((void *)al, sl);
    } else {
        fastboot_fail("Usage: hexdump <addr> <size>");
    }

    fastboot_okay("");
}

void register_commands() {
    fastboot_register("oem hexdump", cmd_hexdump, 1);
}
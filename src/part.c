#include "include/part.h"

part_dev_t *mt_part_get_device(void) {
    return ((part_dev_t *(*)(void))(0x48090d44 | 1))();
}
#include "include/part.h"

// 41e14050
part_dev_t *mt_part_get_device(void) {
    return ((part_dev_t *(*)(void))(0x41e14050 | 1))();
}
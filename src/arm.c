#include "include/arm.h"

unsigned int __aeabi_uidiv(unsigned int n, unsigned int d) {
    unsigned int q = 0;
    while (n >= d) {
        n -= d;
        q++;
    }
    return q;
}
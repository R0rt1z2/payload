#include <stddef.h>

#include "include/memory.h"

typedef long word;

#define lsize sizeof(word)
#define lmask (lsize - 1)

void *malloc(size_t size) {
    return ((void *(*)(size_t))(0x41e21a20 | 1))(size);
}

void free(void *ptr) {
    ((void (*)(void *))(0x41e21194 | 1))(ptr);
}

void *memcpy(void *dest, const void *src, size_t count)
{
    char *d = (char *)dest;
    const char *s = (const char *)src;
    int len;

    if (count == 0 || dest == src)
        return dest;

    if (((long)d | (long)s) & lmask) {
        if ((((long)d ^ (long)s) & lmask) || (count < lsize))
            len = count;
        else
            len = lsize - ((long)d & lmask);

        count -= len;
        for (; len > 0; len--)
            *d++ = *s++;
    }
    for (len = count / lsize; len > 0; len--) {
        *(word *)d = *(word *)s;
        d += lsize;
        s += lsize;
    }
    for (len = count & lmask; len > 0; len--)
        *d++ = *s++;

    return dest;
}

void *memset(void *s, int c, size_t count)
{
    char *xs = (char *) s;
    size_t len = (-(size_t)s) & (sizeof(size_t)-1);
    int cc = c & 0xff;

    if ( count > len ) {
        count -= len;
        cc |= cc << 8;
        cc |= cc << 16;

        for ( ; len > 0; len-- )
            *xs++ = c;

        for ( len = count/sizeof(size_t); len > 0; len-- ) {
            *((size_t *)xs) = cc;
            xs += sizeof(size_t);
        }

        count &= sizeof(size_t)-1;
    }

    for ( ; count > 0; count-- )
        *xs++ = c;

    return s;
}

void *memchr(const void *s, int c, size_t n)
{
    const unsigned char *p = s;
    while (n--) {
        if (*p == (unsigned char)c)
            return (void *)p;
        p++;
    }
    return NULL;
}

int memcmp(const void *cs, const void *ct, size_t count)
{
    const unsigned char *su1, *su2;
    int res = 0;

    for( su1 = cs, su2 = ct; 0 < count; ++su1, ++su2, count-- ) {
        if ((res = *su1 - *su2) != 0)
            break;
    }
    return res;
}

void *memmove(void *dest, void const *src, size_t count)
{
    char *d = (char *)dest;
    const char *s = (const char *)src;
    int len;

    if (count == 0 || dest == src)
        return dest;

    if ((long)d < (long)s) {
        if (((long)d | (long)s) & lmask) {
            if ((((long)d ^ (long)s) & lmask) || (count < lsize))
                len = count;
            else
                len = lsize - ((long)d & lmask);

            count -= len;
            for(; len > 0; len--)
                *d++ = *s++;
        }
        for (len = count / lsize; len > 0; len--) {
            *(word *)d = *(word *)s;
            d += lsize;
            s += lsize;
        }
        for (len = count & lmask; len > 0; len--)
            *d++ = *s++;
    } else {
        d += count;
        s += count;
        if(((long)d | (long)s) & lmask) {
            if((((long)d ^ (long)s) & lmask) || (count <= lsize))
                len = count;
            else
                len = ((long)d & lmask);

            count -= len;
            for(; len > 0; len--)
                *--d = *--s;
        }
        for(len = count / lsize; len > 0; len--) {
            d -= lsize;
            s -= lsize;
            *(word *)d = *(word *)s;
        }
        for(len = count & lmask; len > 0; len--)
            *--d = *--s;
    }

    return dest;
}

void *memscan(void *addr, int c, size_t size)
{
    unsigned char *p = addr;
    while (size) {
        if (*p == c)
            return p;
        p++;
        size--;
    }
    return NULL;
}
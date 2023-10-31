#include <stddef.h>
#include <stdint.h>

#include "include/string.h"
#include "include/memory.h"

char *strcat(char *dest, const char *src)
{
    char *tmp = dest;
    while (*dest)
        dest++;
    while ((*dest++ = *src++) != 0);
    return tmp;
}

char *strchr(const char *s, int c)
{
    for(; *s != (char) c; ++s)
        if (*s == '\0')
            return NULL;
    return (char *) s;
}

int strcmp(char const *cs, char const *ct)
{
    signed char res;
    while(1) {
        if ((res = *cs - *ct++) != 0 || !*cs++)
            break;
    }
    return res;
}

int strcoll(const char *s1, const char *s2)
{
    return strcmp(s1, s2);
}

char *strcpy(char *dest, const char *src)
{
    char *tmp = dest;
    while ((*dest++ = *src++) != '\0');
    return tmp;
}

size_t strlen(const char *str) {
    size_t len = 0;
    while (str[len]) {
        len++;
    }
    return len;
}

char *strdup(const char *s)
{
    size_t len = strlen(s) + 1;
    char *new = malloc(len);
    if (new == NULL)
        return NULL;
    return (char *) memcpy(new, s, len);
}

size_t strlcat(char *dest, const char *src, size_t size)
{
    size_t dlen = strlen(dest);
    size_t slen = strlen(src);
    size_t res = dlen + slen;
    if (dlen >= size)
        return size + slen;
    if (slen < size - dlen)
        memcpy(dest + dlen, src, slen + 1);
    else
        memcpy(dest + dlen, src, size - dlen - 1);
    return res;
}

size_t strlcpy(char *dest, const char *src, size_t size)
{
    size_t i;
    if (!size)
        return strlen(src);
    for (i= 0; ((i < size - 1) && src[i]); i++)
        dest[i] = src[i];
    dest[i] = 0;
    return strlen(src);
}

char *strncat(char *dest, const char *src, size_t n)
{
    char *tmp = dest;
    while (*dest)
        dest++;
    while (n-- && (*dest++ = *src++) != 0);
    if (*dest)
        *dest = '\0';
    return tmp;
}

int strncmp(const char *cs, const char *ct, size_t count)
{
    signed char res = 0;
    while (count) {
        if ((res = *cs - *ct++) != 0 || !*cs++)
            break;
        count--;
    }
    return res;
}

char *strncpy(char *dest, const char *src, size_t n)
{
    char *tmp = dest;
    while (n-- && (*dest++ = *src++) != '\0');
    return tmp;
}

size_t strnlen(char const *s, size_t count)
{
    const char *sc;
    for(sc = s; count-- && *sc != '\0'; ++sc);
    return sc - s;
}

char *strrchr(const char *s, int c)
{
    const char *last = NULL;
    do {
        if (*s == (char) c)
            last = s;
    } while (*s++);
    return (char *) last;
}

size_t strspn(const char *s, const char *accept)
{
    const char *p;
    const char *a;
    size_t count = 0;

    for (p = s; *p != '\0'; ++p) {
        for (a = accept; *a != '\0'; ++a) {
            if (*p == *a)
                break;
        }
        if (*a == '\0')
            return count;
        ++count;
    }

    return count;
}

char *strstr(const char *haystack, const char *needle)
{
    size_t needle_len;
    if (*needle == '\0')
        return (char *) haystack;
    needle_len = strlen(needle);
    for (; (haystack = strchr(haystack, *needle)) != NULL; haystack++) {
        if (strncmp(haystack, needle, needle_len) == 0)
            return (char *) haystack;
    }
    return NULL;
}

char* strtok(char* str, const char* delim) {
    static char* last_token = NULL;
    if (str == NULL) {
        str = last_token;
    }
    if (str == NULL) return NULL;

    str += strspn(str, delim);
    if (*str == '\0') {
        last_token = NULL;
        return NULL;
    }

    char* end = str + strcspn(str, delim);
    if (*end == '\0') {
        last_token = NULL;
    } else {
        *end = '\0';
        last_token = end + 1;
    }
    return str;
}

size_t strcspn(const char *str, const char *reject) {
    const char *p;
    const char *r;
    size_t count = 0;

    for (p = str; *p != '\0'; ++p) {
        for (r = reject; *r != '\0'; ++r) {
            if (*p == *r) {
                return count;
            }
        }
        ++count;
    }
    return count;
}

long strtol(const char* str, char** endptr, int base) {
    const char* p = str;
    long value = 0;
    int sign = 1;

    while (*p == ' ' || *p == '\t' || *p == '\n' || *p == '\v' || *p == '\f' || *p == '\r') {
        ++p;
    }

    if (*p == '+' || *p == '-') {
        sign = (*p++ == '-') ? -1 : 1;
    }

    if ((base == 0 || base == 16) &&
        *p == '0' && (*(p + 1) == 'x' || *(p + 1) == 'X')) {
        p += 2;
        base = 16;
    }

    if (base == 0) {
        base = (*p == '0') ? 8 : 10;
    }

    while (1) {
        int digit;

        if (*p >= '0' && *p <= '9') digit = *p - '0';
        else if (*p >= 'a' && *p <= 'z') digit = *p - 'a' + 10;
        else if (*p >= 'A' && *p <= 'Z') digit = *p - 'A' + 10;
        else break;

        if (digit >= base) break;

        value = value * base + digit;
        ++p;
    }

    if (endptr) *endptr = (char*)p;

    return value * sign;
}

char *w2ascii(const uint16_t* wstr) {
    if (!wstr) return NULL;

    size_t len = 0;
    while (wstr[len]) len++;

    char *ascii = malloc(len + 1);
    if (!ascii) return NULL;
    for (size_t i = 0; i < len; i++) {
        ascii[i] = (wstr[i] <= 0x007F) ? (char) wstr[i] : '?';
    }

    ascii[len] = '\0';
    return ascii;
}

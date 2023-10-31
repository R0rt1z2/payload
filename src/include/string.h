#pragma once

#include <stddef.h>

size_t strlen(const char *str);
char *strdup(const char *s);
size_t strlcat(char *dest, const char *src, size_t size);
size_t strlcpy(char *dest, const char *src, size_t size);
char *strcpy(char *dest, const char *src);
char *strncpy(char *dest, const char *src, size_t n);
char *strncat(char *dest, const char *src, size_t n);
int strcmp(const char *s1, const char *s2);
int strncmp(const char *s1, const char *s2, size_t n);
int strcoll(const char *s1, const char *s2);
char *strchr(const char *s, int c);
char *strrchr(const char *s, int c);
char *strstr(const char *haystack, const char *needle);
char *strtok(char *str, const char *delim);
size_t strspn(const char *s, const char *accept);
size_t strcspn(const char *s, const char *reject);
size_t strnlen(char const *s, size_t count);
long strtol(const char *nptr, char **endptr, int base);
char *w2ascii(const uint16_t* wstr);
int wstrcmp(const uint16_t *wstring, const char *string);
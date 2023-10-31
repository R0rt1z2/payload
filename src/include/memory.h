#pragma once

void *malloc(size_t size);
void free(void *ptr);
void *memcpy(void *dest, const void *src, size_t n);
void *memset(void *s, int c, size_t count);
void *memchr(const void *s, int c, size_t n);
int memcmp(const void *cs, const void *ct, size_t count);
void *memmove(void *dest, void const *src, size_t count);
void *memscan(void *addr, int c, size_t size);
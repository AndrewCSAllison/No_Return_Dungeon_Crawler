#ifndef UTILS_H
#define UTILS_H

#include <stddef.h> // Provides size_t

#define ABS(x) ((x) < 0 ? -(x) : (x))
#define CLAMP(x, lo, hi) ((x) < (lo) ? (lo) : (x) > (hi) ? (hi) : (x))

void* memcpy(void* dest, const void* src, size_t n);
void* memset(void* s, int c, size_t n);

#endif

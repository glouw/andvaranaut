#pragma once

#include "Types.h"

extern int fl(const float x);

extern int cl(const float x);

extern float dec(const float x);

extern int lns(FILE* const file);

extern char* readln(FILE* const file);

extern void bomb(const char* const message, ...);

#define len(array) (int) (sizeof(array) / sizeof(*array))

#define boolean(test) (test ? "true" : "false")

#define toss(t, n) ((t*) malloc((n) * sizeof(t)))

#define retoss(ptr, t, n) (ptr = (t*) realloc((ptr), (n) * sizeof(t)))

#define zero(a) (memset(&a, 0, sizeof(a)))

#define max(a, b) (a > b ? a : b)

#define min(a, b) (a < b ? a : b)

#define odd(a) (a % 2)

#define pi (3.14159265358979323846)

// Minimum array size for integer to string conversions
#define MINTS ((CHAR_BIT * sizeof(int) - 1) / 3 + 2)

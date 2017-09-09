#pragma once

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <limits.h>

int fl(const float x);

int cl(const float x);

float dec(const float x);

int lns(FILE* const file);

char* readln(FILE* const file);

void bomb(const char* const message, ...);

bool odd(const int a);

int balance(const int a);

char* concat(const char* const a, const char* const b);

/*
 * Some generic macros
 */

#define toss(t, n) ((t*) malloc((n) * sizeof(t)))

#define retoss(ptr, t, n) (ptr = (t*) realloc((ptr), (n) * sizeof(t)))

#define zero(a) (memset(&(a), 0, sizeof(a)))

#define max(a, b) ((a) > (b) ? (a) : (b))

#define min(a, b) ((a) < (b) ? (a) : (b))

#define pi (3.14159265358979323846)

// Minimum array size for integer to string conversions
#define MINTS ((CHAR_BIT * sizeof(int) - 1) / 3 + 2)

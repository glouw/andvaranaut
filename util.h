#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include <stdlib.h>
#include <limits.h>

// Fast math floor.
int xfl(const float x);

// Fast math ceil.
int xcl(const float x);

// Fast math decimal.
float xdec(const float x);

int xlns(FILE* const file);

char* xreadln(FILE* const file);

void xbomb(const char* const message, ...);

int xodd(const int a);

#define xlen(a) ((int) (sizeof(a) / sizeof(*a)))

#define xwipe(t, n) ((t*) calloc((n), sizeof(t)))

#define xtoss(t, n) ((t*) malloc((n) * sizeof(t)))

#define xretoss(ptr, t, n) (ptr = (t*) realloc((ptr), (n) * sizeof(t)))

#define xzero(a) (memset(&(a), 0, sizeof(a)))

#define xmax(a, b) ((a) > (b) ? (a) : (b))

#define xmin(a, b) ((a) < (b) ? (a) : (b))

#define xequals(a, b) (strcmp(a, b) == 0)

int xd2(), xd4(), xd8();

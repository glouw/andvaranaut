#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include <stdlib.h>
#include <limits.h>

char* dups(const char *s);

int xfl(const float x);

int xcl(const float x);

float xdec(const float x);

int xlns(FILE* const file);

char* xreadln(FILE* const file);

void xbomb(const char* const message, ...);

int xodd(const int a);

float xsinc(const float x, const float p);

#define xfrand() (rand() / (float) INT_MAX)

#define xlen(a) ((int) (sizeof(a) / sizeof(*a)))

#define xwipe(t, n) ((t*) calloc((n) , sizeof(t)))

#define xtoss(t, n) ((t*) malloc((n) * sizeof(t)))

#define xretoss(ptr, t, n) (ptr = (t*) realloc((ptr), (n) * sizeof(t)))

#define xmax(a, b) ((a) > (b) ? (a) : (b))

#define xmin(a, b) ((a) < (b) ? (a) : (b))

#define xequals(a, b) (strcmp(a, b) == 0)

#define FPI (3.1416f)

int xd2(void), xd4(void), xd8(void), xd10(void);

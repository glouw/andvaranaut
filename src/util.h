#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include <stdlib.h>
#include <limits.h>

char* u_str_dup(const char* s);

int u_char_count(const char* const str, const char delim);

int u_fl(const float x);

int u_cl(const float x);

float u_dec(const float x);

void u_bomb(const char* const message, ...);

int u_odd(const int a);

float u_sinc(const float x, const float p);

#define u_frand() (rand() / (float) INT_MAX)

#define u_len(a) ((int) (sizeof(a) / sizeof(*a)))

#define u_wipe(t, n) ((t*) calloc((n) , sizeof(t)))

#define u_toss(t, n) ((t*) malloc((n) * sizeof(t)))

#define u_retoss(ptr, t, n) (ptr = (t*) realloc((ptr), (n) * sizeof(t)))

#define u_max(a, b) ((a) > (b) ? (a) : (b))

#define u_min(a, b) ((a) < (b) ? (a) : (b))

#define u_equals(a, b) (strcmp(a, b) == 0)

#define U_PI (3.1416f)

//
// DnD dice where 0 is a valid roll.
//

#define u_d2() (rand() % 2)

#define u_d4() (rand() % 4)

#define u_d8() (rand() % 8)

#define u_d10() (rand() % 10)

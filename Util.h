#pragma once

#include "Types.h"
#include <stdlib.h>

int fl(const float x);
int cl(const float x);
int rnd(const float x);
float dec(const float x);
int lns(FILE* const file);
char* readln(FILE* const file);
void bomb(const char* const message, ...);
#define len(array) (int) (sizeof(array) / sizeof(*array))
#define boolean(test) (test ? "true" : "false")
#define toss(t, n) ((t*) malloc((n) * sizeof(t)))

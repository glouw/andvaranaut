#pragma once

#include "Types.h"

// Fast floor
int fl(const float x);
// Fast ceil
int cl(const float x);
// Fast round
int rnd(const float x);
// Fast decimal
float dec(const float x);
// Returns the number of lines in a file
int lns(FILE* const file);
// Reads a line from a file
char* readln(FILE* const file);
// Prints a message and exits
void bomb(const char* const message, ...);
// Returns the length of an array - does not work on pointers to arrays
#define len(array) (int) (sizeof(array) / sizeof(*array))
// Converts 1 to "true" and 0 to "false"
#define boolean(test) (test ? "true" : "false")

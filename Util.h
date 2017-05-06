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
void bomb(const char* const message);

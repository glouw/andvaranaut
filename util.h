#pragma once

#include <stdio.h>
#include <stdbool.h>

// Fast math floor
int fl(const float x);
// Fast math ceil
int cl(const float x);
// Fast math float decimal
float dec(const float x);
int lns(FILE* const file);
bool done();
char* readln(FILE* const file);

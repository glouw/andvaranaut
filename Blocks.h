#pragma once

#include <stdio.h>

typedef struct
{
    char** ceiling;
    char** walling;
    char** floring;
}
Blocks;

Blocks build(FILE* const file, const int rows);

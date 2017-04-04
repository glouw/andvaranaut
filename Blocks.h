#pragma once

#include <stdio.h>

typedef struct
{
    int rows;
    char** ceiling;
    char** walling;
    char** floring;
}
Blocks;

Blocks build(const char* const name);

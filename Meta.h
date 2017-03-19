#pragma once

#include <stdio.h>

typedef struct
{
    int rows;
    int inside;
}
Meta;

Meta retrieve(FILE* const fp);

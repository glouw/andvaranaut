#pragma once

#include <stdio.h>

#include "Blocks.h"

typedef struct
{
    int rows;
    Blocks blocks;
}
Map;

Map open(const char* const path);
void close(const Map map);
Map reopen(const Map map, const char* const path);

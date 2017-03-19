#pragma once

#include <stdio.h>

#include "Meta.h"
#include "Blocks.h"

typedef struct
{
    Meta meta;
    Blocks blocks;
}
Map;

Map open(const char* const path);
void close(const Map map);
Map reopen(const Map map, const char* const path);

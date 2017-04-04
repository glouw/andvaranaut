#pragma once

#include <stdio.h>

#include "Blocks.h"
#include "Sprites.h"

typedef struct
{
    Blocks blocks;
    Sprites sprites;
}
Map;

Map open(const char* const name);
void close(const Map map);
Map reopen(const Map map, const char* const name);

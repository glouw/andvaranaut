#pragma once

#include "Tri.h"
#include "Map.h"

typedef struct
{
    Tri* tri;
    int count;
    int max;
}
Tris;

Map t_generate(const Points);

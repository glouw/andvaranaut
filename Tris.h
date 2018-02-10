#pragma once

#include "Tri.h"
#include "Sdl.h"

typedef struct
{
    Tri* tri;
    int count;
    int max;
}
Tris;

// Generates a map starting at hero point.
Map xtgenerate();

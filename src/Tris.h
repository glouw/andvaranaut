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

Map xtgen(const Points extra);

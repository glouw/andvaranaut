#pragma once

#include "Point.h"

typedef struct
{
    int tile;
    float offset;
    Point where;
}
Hit;

Hit cast(const Point where, const Point direction, char** const walling);

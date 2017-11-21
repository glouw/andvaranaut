#pragma once

#include "Point.h"
#include "Compass.h"
#include "Map.h"

typedef struct
{
    int surface;
    float offset;
    Point where;
    Compass facing;
}
Hit;

Hit xmarch(const Point where, const Point direction, const Map map);

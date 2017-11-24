#pragma once

#include "Point.h"
#include "Compass.h"
#include "Map.h"

typedef struct hit Hit;

struct hit
{
    int surface;
    float offset;
    Point where;
    Hit* next;
};

typedef struct
{
    Hit* ceiling;
    Hit walling;
}
Hits;

Hits xmarch(const Point where, const Point direction, const Map map);

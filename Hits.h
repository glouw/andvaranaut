#pragma once

#include "Point.h"
#include "Compass.h"
#include "Map.h"

typedef struct hit Hit;

struct hit
{
    int surface;
    int changed;
    float offset;
    Point where;
    Compass facing;
    Hit* next;
};

typedef struct
{
    Hit walling;
    Hit* ceiling;
}
Hits;

Hits xmarch(const Point where, const Point direction, const Map map);

void xbreak(const Hits hits);

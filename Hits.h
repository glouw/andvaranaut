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
    // Linked lists.
    // Remember to free these hits as they are used.
    Hit* ceiling;
    Hit* floring;
    // Not a linked list.
    Hit walling;
}
Hits;

Hits xmarch(const Point where, const Point direction, const Map);

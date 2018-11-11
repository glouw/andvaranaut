#pragma once

#include "Point.h"
#include "Map.h"
#include "Hit.h"

typedef struct
{
    // Linked lists for the ceiling and floor renderer.
    // Free these hits as they are used.
    Hit* ceiling;
    Hit* floring;

    // No linked list needed for the wall renderer as the
    // foremost wall will overlap walls behind it.
    Hit walling;
}
Hits;

Hits h_march(const Point where, const Point direction, const Map);

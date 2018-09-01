#pragma once

#include "Point.h"
#include "Compass.h"
#include "Map.h"
#include "Hit.h"

typedef struct
{
    // Linked lists. Remember to free these hits as they are used.
    Hit* ceiling;
    Hit* floring;

    // No linked list needed for the renderer.
    Hit walling;
}
Hits;

Hits xzhits(void);

Hits xmarch(const Point where, const Point direction, const Map);

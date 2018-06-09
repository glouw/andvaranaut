#pragma once

#include "Points.h"
#include "Theme.h"

typedef struct
{
    Point* where;
    Theme* themes;
    int* agents;
    int count;
}
Rooms;

Rooms xrsinit(const Points);

#pragma once

#include "Points.h"
#include "Theme.h"

typedef struct
{
    Point* wheres;
    Theme* themes;
    int* agents;
    int count;
}
Rooms;

Rooms xrsinit(const Points);

#pragma once

#include "Point.h"

typedef enum
{
    E, SE, S, SW, W, NW, N, NE, DIRS
}
Compass;

int inverted(const Compass face);

Compass needle(const Point where, const Point other);

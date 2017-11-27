#pragma once

#include "Point.h"

typedef enum
{
    E, SE, S, SW, W, NW, N, NE, DIRS
}
Compass;

int xinverted(const Compass);

Compass xneedle(const Point where, const Point other);

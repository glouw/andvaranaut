#pragma once

#include "Point.h"

typedef enum
{
    E, SE, S, SW, W, NW, N, NE, DIRS
}
Compass;

bool xinverted(const Compass face);

Compass xneedle(const Point where, const Point other);

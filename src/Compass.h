#pragma once

#include "Point.h"

typedef enum
{
    E, S, W, N, DIRS
}
Compass;

int xinverted(const Compass);

Compass xneedle(const Point where, const Point other);

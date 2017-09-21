#pragma once

#include "Point.h"

typedef enum
{
    E, S, W, N, DIRS
}
Compass;

Compass needle(const Point where, const Point other);

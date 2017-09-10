#pragma once

#include "Point.h"

typedef enum
{
    E, S, W, N, DIRS
}
Compass;

Compass vneedle(const Point vect);
Compass rneedle(const Point where, const Point other);

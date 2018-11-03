#pragma once

#include "Point.h"

typedef enum
{
    E, S, W, N, DIRS
}
Compass;

int c_isinvert(const Compass);

Compass c_needle(const Point where, const Point other);

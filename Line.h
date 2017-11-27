#pragma once

#include "Point.h"

typedef struct
{
    Point a;
    Point b;
}
Line;

// Return a line rotated t radians around the origin.
Line xrotate(const Line, const float t);

// Returns a point on a line by some percentage n (linear interpolation).
Point xlerp(const Line, const float n);

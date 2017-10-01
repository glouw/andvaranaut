#pragma once

#include "Point.h"

typedef struct
{
    Point a;
    Point b;
}
Line;

// Return a line rotated t radians around the origin
Line rotate(const Line line, const float t);

// Returns a point on a line by some percentage n (linear interpolation)
Point lerp(const Line line, const float n);

// Returns the focal ratio of a line
float focal(const Line line);

// Returns the percentage of a line given screen resolution,
// screen x coordinate, normal vector distance xx from the player
// to the point of interest, and player field of view
float fcast(const Line fov, const int res, const int x, const float xx);

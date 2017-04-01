#pragma once

#include "Point.h"

typedef struct
{
    Point a;
    Point b;
}
Line;

Line rotate(const Line line, const float t);
// Linear interpolation
Point lerp(const Line line, const float n);
float focal(const Line line);
// Ceiling caster
float ccast(const Line fov, const int res, const int x);
// Flooring caster
float fcast(const Line fov, const int res, const int x);

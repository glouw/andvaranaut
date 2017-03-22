#pragma once

#include "Point.h"

typedef struct
{
    Point a;
    Point b;
}
Line;

Line rotate(const Line line, const float t);
Point lerp(const Line line, const float n);
float focal(const Line line);
float ccast(const Line fov, const int res, const int x);
float fcast(const Line fov, const int res, const int x);

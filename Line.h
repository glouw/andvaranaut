#pragma once

#include "Point.h"

typedef struct
{
    Point a;
    Point b;
}
Line;

Line xrotate(const Line, const float t);

Point xlerp(const Line, const float n);

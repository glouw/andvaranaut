#pragma once

#include "Point.h"

typedef struct
{
    Point a;
    Point b;
}
Line;

Line l_rotate(const Line, const float t);

Point l_lerp(const Line, const float n);

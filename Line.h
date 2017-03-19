#pragma once

#include "Point.h"

typedef struct
{
    Point a;
    Point b;
}
Line;

Line rotate(const Line l, const double t);
Point lerp(const Line l, const double n);
double focal(const Line l);
double ccast(const Line fov, const int res, const int xx);
double fcast(const Line fov, const int res, const int xx);

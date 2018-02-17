#pragma once

#include "Point.h"

typedef struct
{
    Point* point;
    int count;
    int max;
}
Points;

Points xpsnew(const int max);

Points xpsadd(Points, const Point);

Points xpspop(const Points, const int max);

Point xpsrand(const Points);

Points xpscat(Points, const Points other);

int xpsfind(const Points ps, const Point p);

#pragma once

#include "Point.h"

typedef struct
{
    Point* point;
    int count;
    int max;
}
Points;

Points xzpoints();

Points xpsnew(const int max);

Points xpsadd(Points, const Point);

Points xpsadduq(Points a, const Points b);

Points xpscat(Points, const Points other);

int xpsfind(const Points, const Point p);

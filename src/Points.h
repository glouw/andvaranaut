#pragma once

#include "Point.h"

typedef struct
{
    Point* point;
    int count;
    int max;
}
Points;

Points p_new(const int max);

Points p_append(Points, const Point);

Points p_add_unique(Points a, const Points b);

Points p_cat(Points, const Points other);

int p_find(const Points, const Point);

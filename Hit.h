#pragma once

#include "Point.h"

typedef struct
{
    int tile;
    double offset;
    Point where;
}
Hit;

Hit collision(const Point hook, const Point direction, char** const walling);
Hit cast(const Point ray, const Point direction, char** const walling);

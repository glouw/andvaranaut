#pragma once

#include "Point.h"

typedef struct hit
{
    int surface;
    float offset;
    Point where;
    struct hit* next;
}
Hit;

#pragma once

#include "Point.h"

typedef struct
{
    float acceleration;
    float speed;
    Point direction;
    Point velocity;
    Point where;
    float height;
}
Clouds;

Clouds xrise();

Clouds xblow(Clouds clouds);

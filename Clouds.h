#pragma once

#include "Point.h"

typedef struct
{
    float acceleration;
    float speed;
    Point direction;
    Point velocity;
    Point where;
}
Clouds;

Clouds xrise();

Clouds xblow(Clouds clouds);

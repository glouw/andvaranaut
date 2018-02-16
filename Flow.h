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
Flow;

Flow xstart();

Flow xstream(Flow);

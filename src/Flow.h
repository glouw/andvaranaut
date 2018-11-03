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

Flow f_start(const float height);

Flow f_stream(Flow);

#pragma once

#include "Point.h"
#include "Method.h"

typedef struct
{
    float power;
    Point velocity;
    int hurts;
    Method method;
    int scindex;
    Point reticule;
}
Attack;

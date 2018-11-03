#pragma once

#include "Point.h"
#include "Method.h"

typedef struct
{
    float power;
    Point dir;
    int hurts;
    Method method;
    int scindex;
    Point reticule;
}
Attack;

Attack a_zero(void);

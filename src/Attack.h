#pragma once

#include "Point.h"
#include "Method.h"

typedef struct
{
    float power;
    Point dir;
    int hurts;
    Method method;
    // Scroll index.
    int scindex;
}
Attack;

Attack xzattack();

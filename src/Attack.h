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

    // For ranged attacks a target reticule is used.
    Point reticule;
}
Attack;

Attack xzattack(void);

#pragma once

#include "Point.h"
#include "Method.h"

typedef struct
{
    float power;
    Point dir;
    int hurts;
    Method method;
    int scroll;
}
Attack;

Attack xzattack();

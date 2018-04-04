#pragma once

#include "Point.h"

typedef struct
{
    float power;
    Point dir;
    int hurts;
}
Attack;

Attack xzattack();

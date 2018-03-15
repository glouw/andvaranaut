#pragma once

#include "Point.h"
#include "Gauge.h"
#include "Input.h"

typedef struct
{
    float power;
    Point dir;
    int hurts;
}
Attack;

Attack xgpower(const Gauge, const Input, const Classification c);

#pragma once

#include "Point.h"
#include "Input.h"
#include "Weapon.h"

typedef struct
{
    int top;
    int max;
    Point* points;
    int mx;
    int my;
    float sfactor;
}
Gauge;

Gauge xgnew(const float sfactor);

void xgfree(const Gauge);

Point xgadd(const Gauge);

Gauge xgwind(Gauge, const Weapon, const Input);

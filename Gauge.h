#pragma once

#include "Point.h"
#include "Input.h"

typedef struct
{
    int top;
    int max;
    Point* points;
    int mx;
    int my;
    float sensitivity;
}
Gauge;

Gauge xgnew(const float sensitivity);

void xgfree(const Gauge);

Point xgadd(const Gauge);

Gauge xgwind(Gauge, const Input input);

#pragma once

#include "Point.h"
#include "Input.h"

typedef struct
{
    int count;
    int max;
    Point* points;
    int mx;
    int my;
}
Gauge;

Gauge xzgauge();

Gauge xgnew();

void xgfree(const Gauge);

Gauge xgwind(Gauge, const Input);

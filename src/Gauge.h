#pragma once

#include "Point.h"
#include "Input.h"
#include "Classification.h"

typedef struct
{
    int count;
    int max;
    Point* points;
    int mx;
    int my;
    int wind;
}
Gauge;

Gauge xzgauge();

Gauge xgnew();

void xgfree(const Gauge);

Gauge xgwind(Gauge, const Classification, const Input);

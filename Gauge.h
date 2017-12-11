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
}
Gauge;

Gauge xgnew();

void xgfree(const Gauge g);

Point xgadd(const Gauge g);

Gauge xgwind(Gauge g, const Input input);

#pragma once

#include "Point.h"
#include "Input.h"
#include "Timer.h"

typedef struct
{
    int count;
    int max;
    Point* points;
    int mx;
    int my;
    int ticks;
    int divisor;
}
Gauge;

Gauge xzgauge(void);

Gauge xgnew(void);

void xgfree(const Gauge);

int xgfizzled(const Gauge, const Timer);

Gauge xgwind(Gauge, const Input, const Timer);

Point xgsum(const Gauge, const int count);

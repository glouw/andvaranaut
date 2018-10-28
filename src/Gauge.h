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
}
Gauge;

Gauge xzgauge(void);

Gauge xgnew(void);

void xgfree(const Gauge);

Gauge xgwind(Gauge, const Input, const Timer);

float xgmag(const Gauge, const float damage);

Point xgsum(const Gauge, const int count);

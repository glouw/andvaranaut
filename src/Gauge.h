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
    int warning;
    int ready;
}
Gauge;

Gauge g_new(const int max);

void g_free(const Gauge);

Gauge g_wind(Gauge, const Input, const Timer);

Point g_sum(const Gauge, const int count);

Point g_position(const Gauge);

Point g_velocity(const Gauge);

Point g_acceleration(const Gauge);

int g_fizzled(const Gauge, const Timer);

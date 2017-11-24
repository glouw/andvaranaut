#pragma once

#include "Line.h"

typedef struct
{
    Line trace;
    Point corrected;
    Line fov;
}
Traceline;

float xfcast(const Traceline traceline, const float y, const float yres);

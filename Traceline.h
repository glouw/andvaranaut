#pragma once

#include "Line.h"

typedef struct
{
    Line trace;
    Point corrected;
    Line fov;
}
Traceline;

Point xgfp(const Traceline tl, const int x, const int yres, const float yaw, const float height);

Point xgcp(const Traceline tl, const int x, const int yres, const float yaw, const float height);

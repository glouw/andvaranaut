#pragma once

#include "Line.h"

typedef struct
{
    int bot;
    int top;
    struct
    {
        int bot;
        int top;
    }
    clamped;
}
Projection;

Projection project(const int res, const Line fov, const Point corrected);

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
    int height;
}
Projection;

Projection xproject(const int yres, const Line fov, const Point corrected);

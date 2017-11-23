#pragma once

#include "Line.h"

typedef struct
{
    int bot;
    int top;
}
Clamped;

typedef struct
{
    float bot;
    float top;
    Clamped clamped;
    float height;
}
Projection;

Projection xproject(const int yres, const Line fov, const Point corrected);

Projection xraise(const Projection p, const int yres);

Projection xoverlay(Projection a, const Projection b);

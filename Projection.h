#pragma once

#include "Point.h"

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
    float mid;
    Clamped clamped;
    float height;
}
Projection;

// Creates a line projection suited for the display based on the focal distance
// and corrected normal distance from the hero to the wall.
Projection xproject(const int yres, const float focal, const float shift, const Point corrected);

Projection xstack(const Projection p, const int yres);

Projection xoverlay(Projection a, const Projection b);

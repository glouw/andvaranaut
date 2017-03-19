#pragma once

#include "Line.h"

typedef struct
{
    int bot;
    int top;
    int height;
    struct Clamped
    {
        int bot;
        int top;
    }
    clamped;
}
Wall;

Wall project(const int res, const Line fov, const Point corrected);
Wall raise(const Wall wall, const int res);

#pragma once

#include "Point.h"
#include "Line.h"

typedef struct
{
    Line trace;
    Point corrected;
    Line fov;
}
Traceline;

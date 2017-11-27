#pragma once

#include "Line.h"

typedef struct
{
    Line trace;
    Point corrected;
    Line fov;
}
Traceline;

float xccast(const Traceline, const float y, const float mid);

float xfcast(const Traceline, const float y, const float mid);

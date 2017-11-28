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
    Clamped clamped;
    float size;
    float height;
    int yres;
    float yaw;
}
Projection;

// Creates a line projection suited for the display based on the focal distance
// and corrected normal distance from the hero to the wall.
Projection xproject(const int yres, const float focal, const float yaw, const Point corrected);

Projection xstack(const Projection);

Projection xrocket(const Projection);

float xccast(const Projection, const int x);

float xfcast(const Projection, const int x);

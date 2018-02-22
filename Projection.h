#pragma once

#include "Point.h"
#include "Clamped.h"
#include "Sheer.h"

typedef struct
{
    float bot;
    float top;
    Clamped clamped;
    float size;
    float height;
    int yres;
    float mid;
    Sheer sheer;
}
Projection;

// Creates a line projection suited for the display based on the focal distance
// and corrected normal distance from the hero to the wall.
Projection xproject(const int yres, const int xres, const float focal, const float yaw, const Point corrected, const float height);

Projection xsheer(const Projection, const Sheer);

float xccast(const Projection, const int x);

float xfcast(const Projection, const int x);

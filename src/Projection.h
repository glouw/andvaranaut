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

Projection p_project(const int yres, const int xres, const float focal, const float pitch, const Point corrected, const float height);

Projection p_sheer(const Projection, const Sheer);

float p_ccast(const Projection, const int x);

float p_fcast(const Projection, const int x);

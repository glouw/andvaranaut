#pragma once

#include "Display.h"

typedef struct
{
    Sdl sdl;
    Display display;
    // Scanlines are rotated 90 degrees. An x argument would be passed here
    int y;
}
Scanline;

void xrend(const Scanline sl, const Ray ray, Point* wheres, const Map m, const Torch t, int* moddings);

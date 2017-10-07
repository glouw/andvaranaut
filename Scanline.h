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

void xrend(
    const Scanline* const scanline,
    const Ray* const ray,
    Point* const wheres,
    const Map map,
    const Torch torch,
    int* const moddings);

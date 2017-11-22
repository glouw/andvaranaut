#pragma once

#include "Display.h"

typedef struct
{
    Sdl sdl;
    Display display;
    // Scanlines are rotated 90 degrees. An x argument would be passed here.
    int y;
}
Scanline;

void xwraster(const Scanline sl, const Ray r, const Torch t);

void xfraster(const Scanline sl, const Ray r, const Torch t, Point* wheres, char** floring, int* moddings);

void xcraster(const Scanline sl, const Ray r, Point* wheres, char** ceiling, int* moddings);

void xsraster(const Scanline sl);

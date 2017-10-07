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

void xwrend(const Scanline* scanline, const Ray* ray);

void xfrend(const Scanline* scanline, const Ray* ray, Point* wheres, char** floring);

void xcrend(const Scanline* scanline, const Ray* ray, Point* wheres, char** ceiling);

void xlight(const Scanline* scanline, const Ray* ray, Point* wheres, Torch torch, int* moddings);

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

// Wall rasterer for one scanline.
void xwraster(const Scanline, const Ray);

// Floor rasterer for one scanline.
void xfraster(const Scanline, const Ray, const float yaw, const Map);

// Ceiling rasterer for one scanline.
void xcraster(const Scanline, const Ray, const float yaw, const Map);

// Sky raster for one scanline.
void xsraster(const Scanline, const Ray);

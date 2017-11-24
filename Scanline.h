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
void xwraster(const Scanline sl, const Ray r, const Torch t);

// Floor rasterer for one scanline.
void xfraster(const Scanline sl, const Ray r, const Torch t, const float yaw, const Map map);

// Ceiling rasterer for one scanline.
void xcraster(const Scanline sl, const Ray r, const Torch t, const float yaw, const Map map);

// Sky raster for one scanline.
void xsraster(const Scanline sl, const Ray r, const Torch t, const float yaw, const Clouds clouds);

// Backdrop sky rasterer
void xbraster(const Scanline sl);

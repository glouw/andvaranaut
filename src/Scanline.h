#pragma once

#include "Sdl.h"

typedef struct
{
    Sdl sdl;
    uint32_t* pixels;
    int width;

    // Scanlines are rotated 90 degrees.
    int y;
}
Scanline;

Point s_raster(const Scanline, const Hits, const Hero, const Flow current, const Flow clouds, const Map);

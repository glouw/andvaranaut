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

Point xraster(const Scanline, const Hits, const Hero, const Flow current, const Flow clouds, const Map);

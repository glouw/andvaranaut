#pragma once

#include "Sdl.h"
#include "Hits.h"

typedef struct
{
    Sdl sdl;
    uint32_t* pixels;
    int width;

    //
    // Scanlines are rotated 90 degrees.
    //

    int y;
}
Scanline;

void s_init_clut(void);

Point s_raster(const Scanline, const Hits, const Hero, const Flow current, const Flow clouds, const Map);


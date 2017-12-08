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

// Renders the map to the display from the position of the hero.
// Water current is animated here too.
Point xraster(const Scanline, const Hits, const Hero, const Current, const Map);

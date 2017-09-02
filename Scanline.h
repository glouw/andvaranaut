#pragma once

#include "Sdl.h"
#include "Display.h"

typedef struct
{
    Sdl sdl;
    Display display;
    int y;
}
Scanline;

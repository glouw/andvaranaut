#pragma once

#include "Display.h"

typedef struct
{
    Sdl sdl;
    Display display;
    int y;
}
Scanline;

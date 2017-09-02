#pragma once

#include "Sdl.h"

typedef struct
{
    uint32_t* pixels;
    int width;
}
Display;

Display lock(const Sdl sdl);

void unlock(const Sdl sdl);


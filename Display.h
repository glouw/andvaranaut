#pragma once

#include "Sdl.h"

typedef struct
{
    uint32_t* pixels;
    int width;
}
Display;

Display xlock(const Sdl);

void xunlock(const Sdl);

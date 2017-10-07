#pragma once

#include "SDL2/SDL.h"

typedef struct
{
    SDL_Surface** surface;
    int count;
}
Surfaces;

Surfaces xpull(const uint32_t format);

void xclean(const Surfaces surfaces);

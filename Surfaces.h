#pragma once

#include "lib/SDL2/SDL.h"

typedef struct
{
    SDL_Surface** surface;
    int count;
}
Surfaces;

Surfaces pull(const uint32_t format);

void clean(const Surfaces surfaces);

#pragma once

#include <SDL2/SDL.h>

typedef struct
{
    SDL_Surface** surface;
    int count;
}
Surfaces;

Surfaces pull(const char* const path, const uint32_t format);

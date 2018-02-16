#pragma once

#include <SDL2/SDL.h>

typedef struct
{
    SDL_Surface** surface;
    int count;
}
Surfaces;

// Gets all surfaces from an art configuration file, storing them in RAM.
// The path and name of the art file is hard coded.
// Color key specifies which color to omit when rendering surface.
Surfaces xpull(const uint32_t key);

// Cleans up all surfaces in RAM.
void xclean(const Surfaces);

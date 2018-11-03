#pragma once

#include <SDL2/SDL.h>

typedef struct
{
    SDL_Surface** surface;
    int count;
}
Surfaces;

Surfaces s_pull(void);

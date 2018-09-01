#pragma once

#include <SDL2/SDL.h>

typedef struct
{
    SDL_Surface** surface;
    int count;
}
Surfaces;

Surfaces xzsurf(void);

Surfaces xpull(void);

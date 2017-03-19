#pragma once

#include "Surfaces.h"

typedef struct
{
    Surfaces surfaces;
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* texture;
}
Gpu;

Gpu setup(const int res);
void release(const Gpu gpu);
void present(const Gpu gpu);

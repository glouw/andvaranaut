#pragma once

#include "Surfaces.h"

#include <SDL2/SDL.h>

typedef struct
{
    Surfaces surfaces;
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* texture;
}
Gpu;

SDL_Surface* load(const char* const path, const uint32_t format);
Surfaces pull(const char* const path, const uint32_t format);
Gpu setup(const int res);
void release(const Gpu gpu);
void present(const Gpu gpu);

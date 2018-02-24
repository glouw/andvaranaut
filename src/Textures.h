#pragma once

#include "Surfaces.h"

typedef struct
{
    SDL_Texture** texture;
    int count;
}
Textures;

// Stores a copy of all the surfaces in GPU memory.
Textures xcache(const Surfaces, SDL_Renderer* const renderer);

void xpurge(const Textures);

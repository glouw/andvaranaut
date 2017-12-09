#pragma once

#include "Surfaces.h"

typedef struct
{
    SDL_Texture** texture;
    int count;
}
Textures;

// Stores a copy of all the surfaces in GPU memory - ideal for fast sprite blitting.
Textures xcache(const Surfaces, SDL_Renderer* const renderer);

// Cleans up all textures in GPU memory.
void xpurge(const Textures);

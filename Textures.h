#pragma once

#include "Surfaces.h"

typedef struct
{
    SDL_Texture** texture;
    int count;
}
Textures;

Textures xcache(const Surfaces surfaces, SDL_Renderer* const renderer);

void xpurge(const Textures textures);

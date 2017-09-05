#pragma once

#include "Surfaces.h"

typedef struct
{
    SDL_Texture** texture;
    int count;
}
Textures;

Textures cache(const Surfaces surfaces, SDL_Renderer* const renderer);

void purge(const Textures textures);

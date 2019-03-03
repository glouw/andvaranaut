#pragma once

#include "Texture.h"
#include "Surfaces.h"

typedef struct
{
    Texture* texture;
    int count;
}
Textures;

Textures t_cache(const Surfaces, SDL_Renderer* const);

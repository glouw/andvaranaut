#pragma once

#include "Surfaces.h"

typedef struct
{
    SDL_Texture** texture;
    int count;
}
Textures;

Textures xztex(void);

Textures xcache(const Surfaces, SDL_Renderer* const renderer);

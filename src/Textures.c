#include "Textures.h"

#include "util.h"

Textures t_cache(const Surfaces surfaces, SDL_Renderer* const renderer)
{
    const int count = surfaces.count;
    SDL_Texture** const texture = u_toss(SDL_Texture*, count);
    for(int i = 0; i < count; i++)
        texture[i] = SDL_CreateTextureFromSurface(renderer, surfaces.surface[i]);
    const Textures textures = { texture, count };
    return textures;
}

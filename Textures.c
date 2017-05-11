#include "Textures.h"

#include "Util.h"

Textures cache(const Surfaces surfaces, SDL_Renderer* const renderer)
{
    const int count = surfaces.count;
    SDL_Texture** const texture = toss(SDL_Texture*, count);
    for(int i = 0; i < count; i++)
        texture[i] = SDL_CreateTextureFromSurface(renderer, surfaces.surface[i]);
    const Textures textures = { count, texture };
    return textures;
}

void purge(const Textures textures)
{
    for(int i = 0; i < textures.count; i++)
        SDL_DestroyTexture(textures.texture[i]);
    free(textures.texture);
}

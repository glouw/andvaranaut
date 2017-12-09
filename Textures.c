#include "Textures.h"

#include "util.h"

Textures xcache(const Surfaces surfaces, SDL_Renderer* const renderer)
{
    const int count = surfaces.count;
    SDL_Texture** const texture = xtoss(SDL_Texture*, count);
    // The textures are simply copied from the surfaces.
    // The textures will reside in GPU memory ready for fast sprite blitting.
    for(int i = 0; i < count; i++)
        texture[i] = SDL_CreateTextureFromSurface(renderer, surfaces.surface[i]);
    const Textures textures = { texture, count };
    return textures;
}

void xpurge(const Textures textures)
{
    for(int i = 0; i < textures.count; i++)
        SDL_DestroyTexture(textures.texture[i]);
    free(textures.texture);
}

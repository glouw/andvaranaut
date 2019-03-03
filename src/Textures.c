#include "Textures.h"

#include "util.h"

Textures t_cache(const Surfaces surfaces, SDL_Renderer* const renderer)
{
    const int count = surfaces.count;
    Texture* texture = u_toss(Texture, count);

    for(int i = 0; i < count; i++)
    {
        texture[i].dye[DYE_WHT] = SDL_CreateTextureFromSurface(renderer, surfaces.surface[i].dye[DYE_WHT]);
        texture[i].dye[DYE_RED] = SDL_CreateTextureFromSurface(renderer, surfaces.surface[i].dye[DYE_RED]);
        texture[i].dye[DYE_BRN] = SDL_CreateTextureFromSurface(renderer, surfaces.surface[i].dye[DYE_BRN]);
    }

    const Textures textures = { texture, count };
    return textures;
}

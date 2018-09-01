#include "Textures.h"

#include "util.h"

Textures xztex(void)
{
    static Textures tex;
    return tex;
}

Textures xcache(const Surfaces surfaces, SDL_Renderer* const renderer)
{
    const int count = surfaces.count;
    SDL_Texture** const texture = xtoss(SDL_Texture*, count);

    // The textures are simply copied from the surfaces and reside in GPU memory.
    for(int i = 0; i < count; i++)
        texture[i] = SDL_CreateTextureFromSurface(renderer, surfaces.surface[i]);

    const Textures textures = { texture, count };

    return textures;
}

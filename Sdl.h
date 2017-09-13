#pragma once

#include "Textures.h"
#include "Sprites.h"
#include "SDL2/SDL.h"

typedef struct
{
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* texture;
    int res;
    int fps;
    Surfaces surfaces;
    Textures textures;
}
Sdl;

Sdl setup(const int res, const int fps);

void release(const Sdl sdl);

void render(const Sdl sdl, const Hero hero, const Sprites sprites, const Map map, const int ticks);

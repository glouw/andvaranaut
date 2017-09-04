#pragma once

#include "Surfaces.h"
#include "Textures.h"
#include "Hero.h"
#include "Sprites.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

typedef struct
{
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* texture;
    TTF_Font* font;
    int res;
    int fps;
    Surfaces surfaces;
    Textures textures;
    int renders;
    int ticks;
}
Sdl;

Sdl setup(const int res, const int fps);

void release(const Sdl sdl);

Sdl tick(const Sdl sdl, const int renders);

void render(const Sdl sdl, const Hero hero, const Sprites sprites, const Map map);

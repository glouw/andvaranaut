#pragma once

#include "Textures.h"
#include "Sprites.h"
#include "SDL2/SDL.h"

typedef struct
{
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* texture;
    int xres;
    int yres;
    int fps;
    Surfaces surfaces;
    Textures textures;
}
Sdl;

Sdl setup(const int xres, const int yres, const int fps);

void release(const Sdl sdl);

// Renders one one frame with SDL using hero, sprite, and map data. Ticks determine animation
void render(const Sdl sdl, const Hero hero, const Sprites sprites, const Map map, const int ticks);

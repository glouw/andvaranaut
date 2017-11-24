#pragma once

#include "Textures.h"
#include "Sprites.h"
#include "Args.h"
#include "Overview.h"
#include "Clouds.h"

#include <SDL2/SDL.h>

typedef struct
{
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* canvas;
    int xres;
    int yres;
    int fps;
    Surfaces surfaces;
    Textures textures;
}
Sdl;

Sdl xsetup(const Args args);

void xrelease(const Sdl sdl);

void xpresent(const Sdl sdl);

// Renders one one frame with SDL using hero, sprite, and map data. Ticks determine animation.
void xrender(const Sdl sdl, const Hero hero, const Sprites sprites, const Map map, const Clouds clouds, const int ticks);

// Renders the overview of the map and sprites for editing purposes.
void xview(const Sdl sdl, const Overview ov, const Sprites sprites, const Map map, const int ticks);

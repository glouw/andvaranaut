#pragma once

#include "Textures.h"
#include "Sprites.h"
#include "Args.h"
#include "Overview.h"
#include "Current.h"

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

Sdl xsetup(const Args);

void xrelease(const Sdl);

void xpresent(const Sdl);

// Renders one one frame with SDL using hero, sprite, and map data. Ticks determine animation.
void xrender(const Sdl, const Hero, const Sprites, const Map, const Current, const int ticks);

// Renders the overview of the map and sprites for editing purposes.
void xview(const Sdl, const Overview, const Sprites, const Map, const int ticks);

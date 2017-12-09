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
    // Final renderered frame including walls, ceiling, floors, sprites,
    // and any gui related elements if playing, or the editing screen when editing the map.
    SDL_Renderer* renderer;
    // For per-pixel access when drawing walls, ceiling, and floors.
    SDL_Texture* canvas;
    int xres;
    int yres;
    int fps;
    // All sprite and block surfaces in bitmap format.
    Surfaces surfaces;
    // GPU textures of all the surfaces used predominatly for the sprites.
    Textures textures;
}
Sdl;

Sdl xsetup(const Args);

void xrelease(const Sdl);

void xpresent(const Sdl);

// Renders one frame with SDL using hero, sprite, and map data. Ticks determine sprite animation.
void xrender(const Sdl, const Hero, const Sprites, const Map, const Current, const int ticks);

// Renders the overview of the map and sprites for editing purposes.
void xview(const Sdl, const Overview, const Sprites, const Map, const int ticks);

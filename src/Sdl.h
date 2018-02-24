#pragma once

#include "Textures.h"
#include "Sprites.h"
#include "Args.h"
#include "Overview.h"
#include "Flow.h"
#include "Gauge.h"

#include <SDL2/SDL.h>

typedef struct
{
    SDL_Window* window;
    // Final renderered frame including walls, ceiling, floors, sprites, sprites
    // and any gui related elements if playing, or the editing screen when editing the map.
    SDL_Renderer* renderer;
    // For per-pixel access when drawing walls, ceiling, and floors.
    SDL_Texture* canvas;
    int xres;
    int yres;
    int fps;
    // All sprite and block surfaces in bitmap format stored in RAM with applied color key.
    Surfaces surfaces;
    // All sprite and block surfaces converted from RAM memory in GPU memory with applied color key.
    Textures textures;
    // CPU rendering threads.
    int threads;
    // Color key specifies which colors to omit when rendering sprites or surfaces.
    uint32_t key;
}
Sdl;

Sdl xsetup(const Args);

void xrelease(const Sdl);

void xpresent(const Sdl);

// Renders one frame with SDL using hero, sprite, and map data. Ticks determine sprite animation.
void xrender(const Sdl, const Hero, const Sprites, const Map, const Flow current, const Flow clouds, const int ticks);

// Renders the overview of the map and sprites for editing purposes.
void xview(const Sdl, const Overview, const Sprites, const Map, const int ticks);

void xdgauge(const Sdl, const Gauge);

void xdmap(const Sdl, const Map, const Point);

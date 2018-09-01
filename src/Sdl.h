#pragma once

#include "Textures.h"
#include "Sprites.h"
#include "Timer.h"
#include "Args.h"
#include "Overview.h"
#include "Flow.h"
#include "Gauge.h"
#include "Scroll.h"
#include "Attack.h"
#include "Ttf.h"

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

    // Index of the gui in Surfaces type.
    int gui;

    // Custom gui colors.
    uint32_t wht;
    uint32_t blk;
    uint32_t red;
    uint32_t yel;
}
Sdl;

Sdl xzsdl();

Sdl xsetup(const Args);

void xpresent(const Sdl);

// Renders one frame with SDL using hero, sprite, and map data. Ticks determine sprite animation.
void xrender(const Sdl, const Ttf, const Hero, const Sprites, const Map, const Flow current, const Flow clouds, const Timer);

// Renders the overview of the map and sprites for editing purposes.
void xview(const Sdl, const Overview, const Sprites, const Map, const Timer);

Attack xdgauge(const Sdl, const Gauge, const Inventory, const Scroll);

void xdmap(const Sdl, const Map, const Point);

void xdinv(const Sdl, const Inventory);

void xdbars(const Sdl, const Hero, const Timer);

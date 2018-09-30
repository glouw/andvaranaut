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
#include "Text.h"

#include <SDL2/SDL.h>

typedef struct
{
    // Window via operating system. Renderer is shown through here.
    SDL_Window* window;

    // Final rendered frame including walls, ceiling, floors, sprites, sprites
    // and any GUI related elements if playing, or the editing screen when editing the map.
    SDL_Renderer* renderer;

    // For per-pixel access when software rendering walls, ceiling, and floors.
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

    // Index of the GUI in surfaces.
    int gui;

    // Custom GUI colors.
    uint32_t wht;
    uint32_t blk;
    uint32_t red;
    uint32_t yel;
}
Sdl;

Sdl xzsdl(void);

Sdl xsetup(const Args);

void xpresent(const Sdl);

void xrender(const Sdl, const Text, const Hero, const Sprites, const Map, const Flow current, const Flow clouds, const Timer);

void xview(const Sdl, const Overview, const Sprites, const Map, const Timer);

Attack xdgauge(const Sdl, const Gauge, const Inventory, const Scroll);

void xdmap(const Sdl, const Map, const Point);

void xdinv(const Sdl, const Inventory);

void xdbars(const Sdl, const Hero, const Timer);

void xdfps(const Sdl, const Text, const int fps);

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
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* canvas;
    int xres;
    int yres;
    int fps;
    Surfaces surfaces;
    Textures textures;
    int threads;
    int gui;
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

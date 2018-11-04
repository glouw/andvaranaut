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

Sdl s_setup(const Args);

void s_present(const Sdl);

void s_render_playing(const Sdl, const Text, const Hero, const Sprites, const Map, const Flow current, const Flow clouds, const Inventory inv, const Timer);

void s_render_overlay(const Sdl, const Overview, const Sprites, const Map, const Timer);

Attack s_draw_gauge(const Sdl, const Gauge, const Inventory, const Scroll);

void s_draw_fps(const Sdl, const Text, const char* const fmt, ...);

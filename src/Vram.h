#pragma once

#include "Point.h"
#include "Map.h"

#include <stdint.h>
#include <SDL2/SDL.h>

typedef struct
{
    uint32_t* pixels;
    int width;
}
Vram;

Vram v_lock(SDL_Texture* const);

void v_unlock(SDL_Texture* const);

void v_draw_rooms(const Vram, const Map, const uint32_t in, const uint32_t out);

void v_draw_dot(const Vram, const Point, const int size, const uint32_t in, const uint32_t out);

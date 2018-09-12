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

Vram xvlock(SDL_Texture* const);

void xvunlock(SDL_Texture* const);

void xvdrooms(const Vram, const Map, const uint32_t in, const uint32_t out);

void xvddot(const Vram, const Point, const int size, const uint32_t in, const uint32_t out);

#pragma once

#include "Sdl.h"

#include <SDL2/SDL_ttf.h>

typedef struct
{
    TTF_Font* type;
    SDL_Color out;
    SDL_Color in;
}
Font;

Font xfzero();

Font xfbuild(const char* const path, const int size, const uint32_t color);

void xfwrt(const Font, const Sdl, const int x, const int y, const char* const text);

#pragma once

#include "Sdl.h"

#include <SDL2/SDL_ttf.h>

typedef struct
{
    TTF_Font* type;
    SDL_Color color;
}
Font;

Font xfzero();

Font xfbuild(const char* const path, const int size, const uint32_t color, const int outlined);

void xfwrt(const Font fill, const Font line, const Sdl, const int x, const int y, const char* const text, const int alpha);

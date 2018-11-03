#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

typedef struct
{
    TTF_Font* type;
    SDL_Color color;
}
Font;

Font f_build(const char* const path, const int size, const uint32_t color, const int outline);

void f_put(const Font fill, const Font line, const char* const text, const int alpha, SDL_Renderer* const rend, const int x, const int y);

void f_putbr(const Font fill, const Font line, const char* const text, const int alpha, SDL_Renderer* const rend, const int x, const int y);

void f_print(const Font fill, const Font line, const char* const text, const int alpha, SDL_Renderer* const rend, const int x, const int y);

#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

typedef struct
{
    TTF_Font* type;
    SDL_Color color;
}
Font;

Font xfzero(void);

Font xfbuild(const char* const path, const int size, const uint32_t color, const int outline);

SDL_Rect xfsize(const Font, const char* const string);

SDL_Texture* xtget(const Font, SDL_Renderer* const rend, const int alpha, const char* text);

void xfrender(const Font fill, const Font line, SDL_Renderer* const, const SDL_Rect target, const char* const text, const int alpha);

void xfputxy(const Font fill, const Font line, const char* const text, const int alpha, SDL_Renderer* const rend, const int x, const int y);

void xfputbr(const Font fill, const Font line, const char* const text, const int alpha, SDL_Renderer* const rend, const int x, const int y);

void xfprint(const Font fill, const Font line, const char* const text, const int alpha, SDL_Renderer* const rend, const int x, const int y);

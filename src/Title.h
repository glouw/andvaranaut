#pragma once

#include "Font.h"

#include <SDL2/SDL.h>

typedef struct
{
    int start;
    int now;
    int end;
    char* str;
}
Title;

Title xttzero();

void xttnow(Title*, const int now);

void xttset(Title*, const int start, const int end, const char* const text, ...);

Title* xttnew(const int start, const int end, const char* const text);

void xttshow(Title*, const Font fill, const Font line, SDL_Renderer* const, const int xres, const int yres);

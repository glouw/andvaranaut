#pragma once

#include "Sdl.h"
#include "Font.h"

typedef struct
{
    int start;
    int now;
    int end;
}
Title;

Title xttzero();

Title xttnew(const int start, const int end);

Title xttnow(Title tt, const int now);

Title xttset(Title tt, const int start, const int end);

void xttshow(const Title, const Font fill, const Font line, const Sdl sdl, const char* const text, ...);

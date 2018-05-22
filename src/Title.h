#pragma once

#include "Font.h"
#include "Sdl.h"

void xttadvance(const int now);

void xttset(const int start, const int end, const char* const text, ...);

void xttinit();

void xttshow(const Font fill, const Font line, const Sdl);

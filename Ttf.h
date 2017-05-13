#pragma once

#include "Types.h"

SDL_Rect size(TTF_Font* const font, char* const text);

void scribble(const Ttf ttf, const int x, const int y, const Sdl sdl);

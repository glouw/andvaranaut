#pragma once

#include "Types.h"

extern Sdl setup(const int res, const int fps);

extern void release(const Sdl sdl);

extern Sdl tick(const Sdl sdl, const int renders);

extern void print(const Sdl sdl, const int x, const int y, char* const text);

extern void render(const Sdl sdl, const Hero hero, const Sprites sprites, const Map map);


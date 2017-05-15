#pragma once

#include "Types.h"

Sdl setup(const int res, const int fps);

void release(const Sdl sdl);

Sdl tick(const Sdl sdl, const int renders);

void print(const Sdl sdl, const int x, const int y, char* const text);

void render(const Sdl sdl, const Hero hero, const Sprites sprites, const Map map);

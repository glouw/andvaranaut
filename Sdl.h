#pragma once

#include "Types.h"

Sdl setup(const int res, const int fps, const char* const name);
void release(const Sdl sdl);
Sdl tick(const Sdl sdl, const unsigned long long renders);
void churn(const Sdl sdl);
void present(const Sdl sdl);
void render(const Sdl sdl, const Hero hero, const Sprites sprites, const Map map);

#pragma once

#include "Types.h"

extern Hero spawn();

extern Point touch(const Hero hero, const float reach);

extern Impact march(const Hero hero, char** const block, const Point column, const int res);

extern int teleporting(const Hero hero, const Map map, const Input input, const Sdl sdl);

extern Hero teleport(const Hero hero, const Map map);

extern Hero sustain(const Hero hero, const Sprites sprites, const Map map, const Input input);


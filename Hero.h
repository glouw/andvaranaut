#pragma once

#include "Types.h"

extern Hero spawn();

extern Impact march(const Hero hero, const Trajectory trajectory, const int res);

extern Map edit(const Hero hero, const Map map, const Input input);

extern Hero save(const Hero hero, const Map map, const Sprites sprites, const Input input);

extern Sprites place(const Hero hero, const Sprites sprites, const Input input);

extern Hero sustain(const Hero hero, const Sprites sprites, const Map map, const Input input);


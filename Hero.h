#pragma once

#include "Types.h"

extern Hero spawn();

extern Point touch(const Hero hero, const float reach);

extern Impact march(const Hero hero, const Trajectory trajectory, const int res);

extern bool teleporting(const Hero hero, const Map map, const Input input, const int ticks);

extern Hero teleport(const Hero hero, const Map map);

extern Hero sustain(const Hero hero, const Sprites sprites, const Map map, const Input input);


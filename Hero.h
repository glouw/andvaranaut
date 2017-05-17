#pragma once

#include "Types.h"

Hero spawn(const char* const map);

Impact march(const Hero hero, const Trajectory trajectory, const int res);

Hero type(const Hero hero, const uint8_t* const key);

Hero console(const Hero hero, const uint8_t* const key);

Hero save(const Hero hero, const Map map, const Sprites sprites, const uint8_t* key);

void edit(const Hero hero, const Map map, const uint8_t* const key);

Hero sustain(const Hero hero, const Sprites sprites, const Map map, const uint8_t* key);

Sprites place(const Hero hero, const Sprites sprites, const uint8_t* const key);

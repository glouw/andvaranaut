#pragma once

#include "Types.h"

extern Hero spawn();

extern Hero spin(const Hero hero, const uint8_t* const key);

extern Point touch(const Hero hero, const float reach);

extern Hero move(const Hero hero, char** const walling, const uint8_t* const key);

extern Hero zoom(const Hero hero, const uint8_t* const key);

extern Hero pick(const Hero hero, const uint8_t* const key);

extern Impact march(const Hero hero, const Trajectory trajectory, const int res);

extern Hero type(const Hero hero, const uint8_t* const key);

extern bool scared(const Hero hero, const Sprites sprites);

extern Map edit(const Hero hero, const Map map, const uint8_t* const key);

extern Hero save(const Hero hero, const Map map, const Sprites sprites, const uint8_t* key);

extern Sprites place(const Hero hero, const Sprites sprites, const uint8_t* const key);

extern Hero console(const Hero hero, const uint8_t* const key);

extern Hero sustain(const Hero hero, const Sprites sprites, const Map map, const uint8_t* const key);


#pragma once

#include "Types.h"

Hero spawn();

Impact march(const Hero hero, const Trajectory trajectory, const int res);

Hero type(const Hero hero, const uint8_t* const key);

Hero console(const Hero hero, const uint8_t* const key);

Hero save(const Hero hero, const Map map, const Sprites sprites, const uint8_t* key);

Map edit(const Hero hero, const Map map, const uint8_t* const key);

Sprites place(const Hero hero, const Sprites sprites, const uint8_t* const key);

Hero move(const Hero hero, char** const walling, const uint8_t* const key);

Hero spin(const Hero hero, const uint8_t* const key);

void grab(const Hero hero, const Sprites sprites, const uint8_t* const key);

bool scared(const Hero hero, const Sprites sprites);

void shove(const Sprites sprites);

Hero zoom(const Hero hero, const uint8_t* const key);

Hero pick(const Hero hero, const uint8_t* const key);

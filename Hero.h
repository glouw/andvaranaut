#pragma once

#include "Types.h"

Hero spawn(const char* const name);

int handle(const Hero hero, char** const walling, const uint8_t* const key);

Impact march(const Hero hero, const Range range, const int res);

Hero teleport(const Hero hero, const Portal portal);

Sprites place(const Hero hero, const Sprites sprites, const uint8_t* const key);

Hero console(const Hero hero, const uint8_t* const key);

Hero type(const Hero hero, const uint8_t* const key);

Hero spin(const Hero hero, const uint8_t* const key);

Hero move(const Hero hero, char** const walling, const uint8_t* const key);

Hero zoom(const Hero hero, const uint8_t* const key);

Hero fade(const Hero hero);

void edit(const Hero hero, const Map map, const uint8_t* const key);

void grab(const Hero hero, const Sprites sprites, const uint8_t* const key);

Hero pick(const Hero hero, const uint8_t* const key);

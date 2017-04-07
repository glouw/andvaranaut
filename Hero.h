#pragma once

#include "Types.h"

Hero spawn(const char* const name);
Hero spin(const Hero hero);
Hero move(const Hero hero, char** const walling);
// Opens a portal door
int handle(const Hero hero, char** const walling);
void render(const Hero hero, const Sprites sprites, const Map map, const int res, const Gpu gpu);
// Teleports through a portal door
Hero teleport(const Hero hero, const Portal portal);

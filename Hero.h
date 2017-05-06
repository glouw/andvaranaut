#pragma once

#include "Types.h"

Hero spawn();
int handle(const Hero hero, char** const walling, const uint8_t* key);
Impact march(const Hero hero, char** const block, const Point column, const int res, const int hits);
Hero teleport(const Hero hero, const Portal portal);
Hero touch(const Hero hero, const Map map, const uint8_t* key);

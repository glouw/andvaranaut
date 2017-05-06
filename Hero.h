#pragma once

#include "Types.h"

Hero spawn();
int handle(const Hero hero, char** const walling, const uint8_t* key);
Impact march(const Hero hero, const Range range, const int res);
Hero teleport(const Hero hero, const Portal portal);
Hero touch(const Hero hero, const Map map, const uint8_t* key);

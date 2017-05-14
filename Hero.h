#pragma once

#include "Types.h"

Hero spawn(const char* const name);

int handle(const Hero hero, char** const walling, const uint8_t* const key);

Impact march(const Hero hero, const Range range, const int res);

Hero teleport(const Hero hero, const Portal portal);

Hero touch(const Hero hero, const Map map, const uint8_t* const key);

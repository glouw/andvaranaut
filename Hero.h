#pragma once

#include "Types.h"

Hero spawn();
Hero spin(const Hero hero, const uint8_t* key);
Hero move(const Hero hero, char** const walling, const uint8_t* key);
int handle(const Hero hero, char** const walling, const uint8_t* key);
Impact march(const Hero hero, char** const block, const Point column, const int res, const int hits);
Hero teleport(const Hero hero, const Portal portal);
Hero burn(const Hero hero, const uint8_t* key);
Hero brighten(const Hero hero);
Hero zoom(const Hero hero, const uint8_t* key);

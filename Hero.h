#pragma once

#include "Types.h"

Hero spawn();
Hero spin(const Hero hero);
Hero move(const Hero hero, char** const walling);
int handle(const Hero hero, char** const walling);
Impact march(const Hero hero, char** const block, const Point column, const int res, const int hits);
Hero teleport(const Hero hero, const Portal portal);
Hero zoom(const Hero hero);
Hero burn(const Hero hero);
Hero brighten(const Hero hero);

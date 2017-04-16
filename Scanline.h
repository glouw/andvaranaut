#pragma once

#include "Types.h"

void wrend(const Boundry boundry, const Hit hit);
void frend(const Boundry boundry, Point* const wheres, char** const floring, const Tracery tracery);
void crend(const Boundry boundry, Point* const wheres, char** const ceiling);
void srend(const Scanline scanline, const float percent);

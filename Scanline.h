#pragma once

#include "Types.h"

void wrend(const Scanline scanline, const Wall wall, const Hit hit);
void frend(const Scanline scanline, const Wall wall, Point* const wheres, const Traceline traceline, char** const floring, float* party);
void crend(const Scanline scanline, const Wall wall, Point* const wheres, char** const ceiling);
void brend(const Scanline scanline);
void srend(const Scanline scanline, const float percent);

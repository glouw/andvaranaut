#pragma once

#include "Types.h"

void wrend(const Boundary boundary, const Hit hit, const int modding);
void frend(const Boundary boundary, Point* const wheres, char** const floring, int* const moddings, const Tracery tracery);
void crend(const Boundary boundary, Point* const wheres, char** const ceiling, int* const moddings);
void srend(const Boundary boundary, const float percent, const int ticks);

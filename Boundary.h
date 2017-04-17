#pragma once

#include "Types.h"

void wrend(const Boundary boundary, const Hit hit);
void frend(const Boundary boundary, Point* const wheres, char** const floring, const Tracery tracery);
void crend(const Boundary boundary, Point* const wheres, char** const ceiling);
void srend(const Boundary boundary, const Hero hero);

#pragma once

#include "Geom.h"

typedef struct hero
{
    struct point where;
    double theta;
    double d0, dy, dx;
}
Hero;

Hero
Hero_Move(Hero hero);

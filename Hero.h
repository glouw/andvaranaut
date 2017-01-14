#pragma once

#include "Geom.h"
#include "Map.h"

typedef struct hero
{
    Point where;
    double theta;
    double d0;
    double dy;
    double dx;
    double health;
}
Hero;

Hero
Hero_Move(Hero hero, Map map);

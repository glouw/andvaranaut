#pragma once

#include "Line.h"
#include "Hit.h"
#include "Blocks.h"
#include "Hero.h"
#include "Gpu.h"

typedef struct
{
    int inside;
    Line fov;
    Point where;
    Point velocity;
    double speed;
    double acceleration;
    double theta;
}
Hero;

Hero spin(const Hero hero);
Hero move(const Hero hero, char** const walling);
Hit shoot(const Hero hero, char** const walling);
int handle(const Hero hero, char** const walling);
void render(const Hero hero, const Blocks blocks, const int res, const Gpu gpu);

#pragma once

#include "Line.h"
#include "Gpu.h"
#include "Portals.h"
#include "Blocks.h"

#include <stdbool.h>

typedef struct
{
    Line fov;
    Point where;
    Point velocity;
    float speed;
    float acceleration;
    struct { float theta, percent; } angle;
}
Hero;

Hero spawn(const char* const path);
Hero spin(const Hero hero);
Hero move(const Hero hero, char** const walling);
// Opens a portal door
int handle(const Hero hero, char** const walling);
void render(const Hero hero, const Blocks blocks, const int res, const Gpu gpu);
// Teleports through a portal door
Hero teleport(const Hero hero, const Portal portal);

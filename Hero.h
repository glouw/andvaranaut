#pragma once

#include "Line.h"
#include "Torch.h"
#include "Ray.h"
#include "Map.h"
#include "Input.h"

#include <stdbool.h>

typedef struct
{
    Line fov;
    Point where;
    Point velocity;
    float speed;
    float acceleration;
    float theta;
    Torch torch;
    // The longer the arm, the further the hero can grab and hit things
    float arm;
    // Dungeon floor hero is currently on
    int floor;
    // Stronger scent will have sprites chase hero from further away
    int scent;
    float yaw;
}
Hero;

Hero xspawn(const float focal);

Point xtouch(const Hero hero, const float reach);

bool xteleporting(const Hero hero, const Map map, const Input input, const int ticks);

Hero xteleport(Hero hero, const Map map);

Hero xsustain(Hero hero, const Map map, const Input input);

Ray xcalc(const Hero hero, const Hit hit, const int yres);

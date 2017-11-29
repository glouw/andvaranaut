#pragma once

#include "Line.h"
#include "Torch.h"
#include "Ray.h"
#include "Map.h"
#include "Input.h"

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
    float height;
}
Hero;

Hero xspawn(const float focal);

Point xtouch(const Hero, const float reach);

int xteleporting(const Hero, const Map, const Input, const int ticks);

Hero xteleport(Hero, const Map);

Hero xsustain(Hero, const Map, const Input);

Ray xcalc(const Hero hero, const Hit hit, const int upper, const int yres);

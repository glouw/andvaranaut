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
    float arm;
    int floor;
    int scent;
    float yaw;
    float height;
    float tall;
    float vvel;
}
Hero;

Hero xspawn(const float focal);

Point xtouch(const Hero, const float reach);

int xteleporting(const Hero, const Map, const Input, const int ticks);

Hero xteleport(Hero, const Map);

Hero xsustain(Hero, const Map, const Input);

Ray xcalc(const Hero hero, const Hit, const float level, const int yres);

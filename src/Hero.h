#pragma once

#include "Sheer.h"
#include "Method.h"
#include "Line.h"
#include "Timer.h"
#include "Torch.h"
#include "Ray.h"
#include "Map.h"
#include "Input.h"
#include "Flow.h"
#include "Hits.h"

typedef struct
{
    Line fov;
    Point where;
    Point velocity;
    float speed;
    float acceleration;
    float theta;
    Torch torch;
    float reach;
    int floor;
    int aura;
    float yaw;
    float vyaw;
    float height;
    float tall;
    float vvel;
    float hps;
    float hpsmax;
    float mna;
    float mnamax;
    float ftg;
    float ftgmax;
    float warning;
    int teleported;
    int teleporting;
}
Hero;

Hero xzhero(void);

Hero xspawn(const float focal, const Point where, const int floor);

Point xtouch(const Hero);

Hero xteleporting(Hero, const Map, const Input, const Timer);

Hero xteleport(Hero, const Map);

Hero xsustain(Hero, const Map, const Input, const Flow current, const Timer);

Ray xcalc(const Hero, const Hit, const Sheer, const int yres, const int xres);

Hero xrecoil(Hero, const Method);

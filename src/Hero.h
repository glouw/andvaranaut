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
#include "Gauge.h"

typedef struct
{
    Line fov;
    Point where;
    Point velocity;
    float speed;
    float acceleration;
    float yaw;
    float dyaw; // Rate of change of yaw (TODO: IMPLEMENT)
    float pitch;
    float dpitch; // Rate of change of pitch.
    Torch torch;
    float reach;
    int floor;
    int aura;
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

Hero xspawn(const float focal, const Point where, const int floor, const Gauge);

Point xtouch(const Hero);

Hero xteleporting(Hero, const Map, const Input, const Timer);

Hero xteleport(Hero, const Map);

Hero xsustain(Hero, const Map, const Input, const Flow, const Method);

Ray xcalc(const Hero, const Hit, const Sheer, const int yres, const int xres);

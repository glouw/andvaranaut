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
#include "Hit.h"
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

Hero h_birth(const float focal, const Point where, const int floor, const Gauge);

Point h_touch(const Hero);

Hero h_teleporting(Hero, const Map, const Input, const Timer);

Hero h_teleport(Hero, const Map);

Hero h_sustain(Hero, const Map, const Input, const Flow, const Method);

Ray h_cast(const Hero, const Hit, const Sheer, const int yres, const int xres);

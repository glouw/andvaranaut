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
    // Field of view.
    Line fov;

    // Vectors.
    Point where;
    Point velocity;

    // Max speed and acceleration scalars.
    float speed;
    float acceleration;

    // Rotation in radians.
    float theta;

    Torch torch;
    float reach;
    int floor;

    // A larger aura will attract sprites from further away.
    int aura;

    // Head up and down.
    float yaw;

    // Velocity yaw from recoil.
    float vyaw;

    // Height varies with jumping, crouching, and swimming.
    float height;

    // Tallness does not vary. Height references tallness for jumping and crouching.
    float tall;

    // Vertical velocity when jumping and falling.
    float vvel;

    // Hitpoints.
    float hps;
    float hpsmax;

    // Mana.
    float mna;
    float mnamax;

    // Fatigue.
    float ftg;
    float ftgmax;

    // When fatigue is low, warning indicators go off (fatigue bar flashes and hero starts to breathe heavily).
    float warning;
}
Hero;

Hero xzhero(void);

Hero xspawn(const float focal, const Point where, const int floor);

Point xtouch(const Hero);

int xteleporting(const Hero, const Map, const Input, const Timer);

Hero xteleport(Hero, const Map);

Hero xsustain(Hero, const Map, const Input, const Flow current, const Timer);

Ray xcalc(const Hero hero, const Hit, const Sheer, const int yres, const int xres);

Hero xrecoil(Hero, const Method);

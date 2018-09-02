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

    // Scalars.
    float speed;
    float acceleration;

    // Rotation in radians.
    float theta;

    // Lighting from player.
    Torch torch;

    // Player arm length.
    float reach;

    // Player floor level.
    int floor;

    // Sprite attraction strength.
    int aura;

    // Panning head up and down.
    float yaw;

    // Velocity yaw up and down from recoil.
    float vyaw;

    // Player height relative to ground which varies with jumping, crouching, and swimming.
    float height;

    // Player tallness which does not vary. Height references tallness for jumping and crouching.
    float tall;

    // Vertical velocity when jumping and falling.
    float vvel;

    // Hitpoints and max hitpoints
    float hps;
    float hpsmax;

    // Mana and max mana.
    float mna;
    float mnamax;

    // Fatigue and max fatigue.
    float ftg;
    float ftgmax;

    // When fatigue is low, warning indicators go off; the fatigue bar will start
    // to flash and the hero will start to breathe heavily, moving the screen up and down.
    float warning;
}
Hero;

Hero xzhero(void);

Hero xspawn(const float focal, const Point where, const int floor);

Point xtouch(const Hero);

int xteleporting(const Hero, const Map, const Input, const Timer);

Hero xteleport(Hero, const Map);

Hero xsustain(Hero, const Map, const Input, const Flow current, const Timer);

Ray xcalc(const Hero, const Hit, const Sheer, const int yres, const int xres);

Hero xrecoil(Hero, const Method);

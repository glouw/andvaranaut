#pragma once

#include "Sheer.h"
#include "Line.h"
#include "Title.h"
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
    // Position and velocity vectors.
    Point where;
    Point velocity;
    // Max speed.
    float speed;
    float acceleration;
    // Rotation (radians).
    float theta;
    Torch torch;
    float reach;
    int floor;
    // A larger aura will attract sprites from further away.
    int aura;
    // Head up and down.
    float yaw;
    // Height varies.
    float height;
    // Tallness does not vary. Height references tallness for jumping and crouching.
    float tall;
    // Vertical velocity.
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
    // Prompts.
    int swam;
}
Hero;

Hero xzhero();

Hero xspawn(const float focal, const Point where, const int floor);

Point xtouch(const Hero);

int xteleporting(const Hero, const Map, const Input, const Timer);

Hero xteleport(Hero, const Map);

Hero xsustain(Hero, const Map, const Input, const Flow, Title* tt, const Timer);

// Calculates a projection ray based on a wall hit. Shift determines the ceiling height if positive or the water height if negative.
Ray xcalc(const Hero hero, const Hit, const Sheer, const int yres, const int xres);

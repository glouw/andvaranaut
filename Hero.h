#pragma once

#include "Weapon.h"
#include "Sheer.h"
#include "Line.h"
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
    float arm;
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
    // Health.
    float hp;
    float hpmax;
    // Mana.
    float mana;
    float manamax;
    // Fatigue
    float ftg;
    float ftgmax;
    // TBD.
    Weapon wep;
    // True if hero is looking at inventory. False otherwise.
    int inventory;
}
Hero;

Hero xspawn(const float focal, const Point where, const int floor);

Point xtouch(const Hero, const float reach);

int xteleporting(const Hero, const Map, const Input, const int ticks);

Hero xteleport(Hero, const Map);

Hero xsustain(Hero, const Map, const Input, const Flow);

// Calculates a projection ray based on a wall hit. Shift determines the ceiling height if positive or the water height if negative.
Ray xcalc(const Hero hero, const Hit, const Sheer, const int yres, const int xres);

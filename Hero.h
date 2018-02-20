#pragma once

#include "Weapon.h"
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
}
Hero;

// Spawns the hero with default settings. Hero will always spawn at the top left of the map (1.5, 1.5).
Hero xspawn(const float focal, const Point where, const int floor);

// Returns a point within arm's length in front of the hero.
Point xtouch(const Hero, const float reach);

// Retruns true if the hero is teleporting.
int xteleporting(const Hero, const Map, const Input, const int ticks);

// Teleports the hero to either a floor above or below.
Hero xteleport(Hero, const Map);

// Hero self sustanance. Does not include sprite to hero interactions.
Hero xsustain(Hero, const Map, const Input, const Flow);

// Calculates a projection ray based on a wall hit.
// Shift determines the ceiling height if positive or the water height if negative.
Ray xcalc(const Hero hero, const Hit, const float a, const float b, const int yres, const int xres);

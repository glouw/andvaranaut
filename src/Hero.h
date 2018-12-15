#pragma once

#include "Sheer.h"
#include "Method.h"
#include "Attack.h"
#include "State.h"
#include "Inventory.h"
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
    float d_yaw; // Rate of change of yaw.
    float pitch;
    float d_pitch; // Rate of change of pitch.
    Torch torch;
    int floor;
    int aura;
    float height;
    float tall;
    float vertical_velocity;
    float health;
    float health_max;
    float mana;
    float mana_max;
    float fatigue;
    float fatigue_max;
    float warning;
    int teleported;
    int teleporting;
    Gauge gauge;
    Inventory inventory;
    Attack attack;
}
Hero;

Hero h_birth(const float focal, const Map[], const int floor);

Hero h_teleporting(Hero, const Map, const Input, const Timer);

Hero h_teleport(Hero, const Map);

Hero h_sustain(Hero, const Map, const Input, const Flow, const Method, const Timer);

Ray h_cast(const Hero, const Hit, const Sheer, const int yres, const int xres);

Hero h_struck(Hero, const State, const float damage);

int h_close_enough(const Hero, const Point other);

#pragma once

#include "Line.h"
#include "Torch.h"
#include "Weapon.h"
#include "Attack.h"
#include "Impact.h"
#include "Map.h"
#include "Input.h"

#include <stdbool.h>

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
    Weapon weapon;
    Attack attack;
}
Hero;

Hero spawn();

Point touch(const Hero hero, const float reach);

Impact march(const Hero hero, char** const block, const Point column, const int res);

bool teleporting(const Hero hero, const Map map, const Input input, const int ticks);

Hero teleport(const Hero hero, const Map map);

Hero sustain(const Hero hero, const Map map, const Input input);

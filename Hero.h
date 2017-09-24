#pragma once

#include "Line.h"
#include "Torch.h"
#include "Ray.h"
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
}
Hero;

Hero spawn(const float scale);

Point touch(const Hero hero, const float reach);

Ray cast(const Hero hero, char** const block, const Point column, const int res);

bool teleporting(const Hero hero, const Map map, const Input input, const int ticks);

Hero teleport(const Hero hero, const Map map);

Hero sustain(const Hero hero, const Map map, const Input input);

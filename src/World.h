#pragma once

#include "Sprites.h"
#include "Map.h"

typedef struct
{
    Map* map;
    Sprites* sprites;
    int last_floor;
    int index;
    int max;
}
World;

World w_make(const int max);

Hero w_interact(const World, Hero, const Timer);

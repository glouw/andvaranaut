#pragma once

#include "Sprites.h"
#include "Map.h"

typedef struct
{
    Map* map;
    Sprites* sprites;
    int floor;
    int floors;
}
World;

World w_make(const int floors);

Hero w_interact(const World, Hero, const Input, const Timer);

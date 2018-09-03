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

// Builds a world with the number of floors specified.
World xwinit(const int floors);

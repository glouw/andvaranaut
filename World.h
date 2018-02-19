#pragma once

#include "Sprites.h"
#include "Map.h"

typedef struct
{
    Map* map;
    Sprites* sprites;
    int count;
    int max;
}
World;

World xwadd(World, const Map, const Sprites);

World xwinit(const int max);

void xwclose(const World);

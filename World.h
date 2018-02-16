#pragma once

#include "Map.h"

typedef struct
{
    Map* map;
    int count;
    int max;
}
World;

World xwadd(World, const Map);

World xwinit(const int max);

void xwclose(const World);

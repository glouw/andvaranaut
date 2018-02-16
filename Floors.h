#pragma once

#include "Map.h"

typedef struct
{
    Map* map;
    int count;
    int max;
}
Floors;

Floors xfsadd(Floors, const Map);

Floors xfsinit(const int max);

void xfsclose(const Floors);

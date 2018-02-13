#pragma once

#include "Map.h"

typedef struct
{
    Map* map;
    int count;
    int max;
}
Maps;

Maps xmsadd(Maps, const Map);

Maps xmsinit(const int floors);

void xmsclose(const Maps);

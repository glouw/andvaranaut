#pragma once

#include "Points.h"
#include "Theme.h"

typedef struct
{
    Point where;
    Theme theme;
    int agents;
    int floor;
}
Room;

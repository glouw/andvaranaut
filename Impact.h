#pragma once

#include "Traceline.h"
#include "Wall.h"
#include "Hit.h"

typedef struct
{
    Traceline traceline;
    Wall wall;
    Hit hit;
}
Impact;

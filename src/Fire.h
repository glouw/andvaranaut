#pragma once

#include "Embers.h"
#include "Map.h"

typedef struct
{
    Embers** embers;
    int rows;
    int cols;
}
Fire;

Fire f_kindle(const Map);

void f_clear(const Fire);

void f_extinguish(const Fire);

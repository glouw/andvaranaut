#pragma once

#include "Embers.h"
#include "Sprites.h"
#include "Hero.h"

typedef struct
{
    Embers** embers;
    int rows;
    int cols;
}
Fire;

Fire f_kindle(const Map);

Fire f_track(Fire, const Sprites);

void f_clear(const Fire);

void f_burn(const Fire, const Hero);

void f_extinguish(const Fire);

#pragma once

#include "Sdl.h"

typedef struct
{
    Map* map;
    Sprites* sprites;
    int index;
    int max;
}
World;

World w_make(const int max);

Hero w_interact(const World, Hero, const Timer);

Hero w_transport(const World, const Hero, const Sdl, const Text red, const Text yel, const Input);

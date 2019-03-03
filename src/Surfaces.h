#pragma once

#include "Surface.h"
#include "Palette.h"

typedef struct
{
    Surface* surface;
    int count;
}
Surfaces;

Surfaces s_load_surfaces(const Palette);

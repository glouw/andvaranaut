#pragma once

#include "Sprite.h"

typedef struct
{
    Sprite** ember; // Non-contiguous ember reference in sprite array.
    int count;
    int max;
}
Embers;

Embers e_append(Embers, Sprite* const ember);

Embers e_reset(const Embers);

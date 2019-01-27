#pragma once

#include "Sprite.h"

//
// Non-contiguous ember reference in sprite array.
//

typedef struct
{
    Sprite** ember;
    int count;
    int max;
}
Embers;

Embers e_append(Embers, Sprite* const ember);

Embers e_reset(const Embers);

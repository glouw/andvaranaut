#pragma once

#include "Projection.h"
#include "Torch.h"

// Rays go from the position of the player to the hit surface of a wall.
// Included is the surface and surface offset, the corrected (normal)
// distance, and the torch of the hero for the pixel shading of the wall.

typedef struct
{
    Line trace;
    Point corrected;
    Projection proj;
    int surface;
    float offset;
    Torch torch;
}
Ray;

#pragma once

#include "Projection.h"
#include "Torch.h"

// Rays go from the position of the player to the hit position of a wall.
// Included is the surface and surface offset of the hit position, the corrected (normal)
// distance from the hit to the player, and the torch of the hero for the later scanline
// calculations of wall pixel shading.

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

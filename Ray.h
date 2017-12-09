#pragma once

#include "Projection.h"
#include "Hits.h"
#include "Torch.h"

typedef struct
{
    Line trace;
    Point corrected;
    Projection proj;
    Hit hit;
    Torch torch;
}
Ray;

#pragma once

#include "Traceline.h"
#include "Projection.h"
#include "Hits.h"
#include "Torch.h"

typedef struct
{
    Traceline traceline;
    Projection proj;
    Hit hit;
    Torch torch;
}
Ray;

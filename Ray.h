#pragma once

#include "Traceline.h"
#include "Projection.h"
#include "Hits.h"

typedef struct
{
    Traceline traceline;
    Projection proj;
    Hit hit;
}
Ray;

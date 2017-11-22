#pragma once

#include "Traceline.h"
#include "Projection.h"
#include "Hits.h"

typedef struct
{
    Traceline traceline;
    Projection projection;
    Hit hit;
}
Ray;

Ray xraise(Ray ray, const int yres);

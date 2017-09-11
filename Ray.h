#pragma once

#include "Traceline.h"
#include "Projection.h"
#include "Hit.h"

typedef struct
{
    Traceline traceline;
    Projection projection;
    Hit hit;
}
Ray;

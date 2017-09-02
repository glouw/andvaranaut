#pragma once

#include "Point.h"

typedef struct
{
    Point vect;
    union
    {
        int swing;
    }
    type;
    float power;
    Point where;
    float reach;
    float area;
}
Attack;

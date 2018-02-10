#pragma once

#include "Points.h"

typedef struct
{
    Point a;
    Point b;
    Point c;
    Point d;
    Point e;
}
Trapdoor;

Trapdoor xtdnew(const Points);

Point xtdrand(const Trapdoor);

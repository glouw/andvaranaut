#pragma once

#include "Map.h"
#include "Hero.h"

typedef struct
{
    float** mesh;
    int rows;
    int cols;
    // If the field resolution is 1 then pathfinding matches the map 1:1.
    // Higher resolutions allow for more precise sprites movements at the
    // expense of sprites grouping up closer to each other.
    // Higher resolutions also eats more CPU cycles.
    int res;
    int aura;
}
Field;

Field xzfield();

// Collaborative Diffusion path finding.
void xdiffuse(const Field, const Point where);

Field xprepare(const Map, const float aura);

Point xforce(const Field, const Point from, const Point to, const Map map);

void xexamine(const Field);

void xdeposit(const Field, const Point p, const float val);

void xruin(const Field);

int xon(const Field, const int y, const int x);

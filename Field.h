#pragma once

#include "Map.h"
#include "Hero.h"

// Collabortive diffusion path finding.

typedef struct
{
    float** mesh;
    int rows;
    int cols;
    // If the field resolution is one then pathfinding matches the map 1:1.
    // If its higher, theres more resolution for the sprites to move around
    // on a single tile.
    // However, the higher this resolution goes, the more taxing pathfinding
    // becomes as the entire field must be diffused per frame.
    int res;
    int aura;
}
Field;

// Collaborative Diffusion.
void xdiffuse(const Field, const Point where);

Field xprepare(const Map, const float aura);

Point xforce(const Field, const Point from, const Point to, const Map map);

void xexamine(const Field, const int less);

void xdeposit(const Field, const Point p, const float val);

void xruin(const Field);

int xon(const Field field, const int y, const int x);

#pragma once

#include "Map.h"
#include "Hero.h"

// Path finding. Uses collabortive diffisuion.

typedef struct
{
    float** mesh;
    int rows;
    int cols;
    int res;
    int aura;
}
Field;

// Collaborative Diffusion.
void xdiffuse(const Field, const Point where);

Field xprepare(const Map, const float aura);

Point xforce(const Field, const Point from, const Point to);

void xexamine(const Field);

void xdeposit(const Field, const Point p, const float val);

void xruin(const Field);

int xon(const Field field, const int y, const int x);

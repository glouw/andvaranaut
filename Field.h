#pragma once

#include "Map.h"
#include "Hero.h"

typedef struct
{
    float** mesh;
    int rows;
    int cols;
    int res;
    int aura;
}
Field;

void xdiffuse(const Field, const Point where);

Field xprepare(const Map, const float scent);

Point xforce(const Field, const Point from, const Point to);

void xexamine(const Field);

void xdeposit(const Field, const Point p, const float val);

void xruin(const Field);

#pragma once

#include "Map.h"
#include "Hero.h"

typedef struct
{
    float** mesh;
    int rows;
    int cols;
    float anti;
    int res;
}
Field;

void xdiffuse(const Field field, const Point where);

Field xprepare(const Map map);

Point xforce(const Field field, const Point from, const Point to);

void xexamine(const Field field);

void xdeposit(const Field field, const Point p, const float val);

void xruin(const Field field);

#pragma once

#include "Map.h"
#include "Hero.h"

typedef struct
{
    float** mesh;
    int rows;
    int cols;
    float anti;
}
Field;

void diffuse(const Field field, const int y, const int x);

Field prepare(const Map map);

Point force(const Field field, const Point from, const Point to);

void examine(const Field field);

void ruin(const Field field);

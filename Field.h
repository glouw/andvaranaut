#pragma once

#include "Map.h"
#include "Hero.h"

typedef struct
{
    float** mesh;
    int rows;
    int cols;
}
Field;

void diffuse(const Field field, const int y, const int x);

Point force(const Field field, const Point p);

Field prepare(const Map map);

void examine(const Field field);

void ruin(const Field field);

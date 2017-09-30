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

void diffuse(const Field field, const Point where);

Field prepare(const Map map);

Point force(const Field field, const Point from, const Point to);

void examine(const Field field);

void deposit(const Field field, const Point p, const float val);

void ruin(const Field field);

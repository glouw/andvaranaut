#pragma once

#include "Map.h"
#include "Hero.h"

typedef struct
{
    float** field;
    int rows;
    int cols;
}
Path;

void diffuse(const Path path, const int y, const int x);

Path prepare(const Map map);

void examine(const Path path);

void ruin(const Path path);

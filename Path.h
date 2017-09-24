#pragma once

#include "Map.h"
#include "Hero.h"

typedef struct
{
    float** density;
    int** clear;
    int rows;
    int cols;
}
Path;

void diffuse(const Path path, const int y, const int x);

Path prepare(const Map map);

void ruin(const Path path);

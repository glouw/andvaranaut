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

Path prepare(const Map map);

void ruin(const Path path);

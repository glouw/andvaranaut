#pragma once

#include "Map.h"
#include "Hero.h"

typedef struct
{
    float** volatile density;
    int rows;
    int cols;
}
Path;

Path prepare(const Map map);

void ruin(const Path path);

Path redo(const Path path, const Map map);

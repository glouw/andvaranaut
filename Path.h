#pragma once

#include "Map.h"
#include "Hero.h"
#include "Sprites.h"

typedef struct
{
    float** volatile hill;
    int rows;
    int cols;
}
Path;

Path prepare(const Map map);

void find(const Path path, const Map map, const Hero hero, const Sprites sprites);

void ruin(const Path path);

Path redo(const Path path, const Map map);

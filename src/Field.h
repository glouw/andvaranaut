#pragma once

#include "Map.h"

typedef struct
{
    float** mesh;

    int rows;
    int cols;

    //
    // If the field resolution is 1 then pathfinding matches the map 1:1.
    // Higher resolutions allow for more precise sprites movements at the
    // expense of sprites grouping up closer to each other.
    // Higher resolutions eat exponentially more CPU cycles.
    //

    int res;
    int aura;
}
Field;

void f_diffuse(const Field, const Point where);

Field f_prepare(const Map, const float aura);

Point f_generate_force(const Field, const Point from, const Point to, const Map map);

void f_deposit(const Field, const Point p, const float val);

void f_ruin(const Field);

int f_is_on(const Field, const int y, const int x);

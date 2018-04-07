#pragma once

#include "Surfaces.h"

typedef struct
{
    int* casting;
    int** castables;
    int grid;
    int squares;
    int count;
}
Scroll;

Scroll xscnew(const Surfaces);

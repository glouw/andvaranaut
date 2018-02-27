#pragma once

#include "Classification.h"

typedef struct
{
    Classification classification;
    int indices;
}
Item;

void xiprint(const Surfaces);

Item xirand(const Surfaces);

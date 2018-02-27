#pragma once

#include "Classification.h"

typedef struct
{
    Classification classification;
    int index;
}
Item;

void xitprint(const Surfaces);

Item xitrand(const Surfaces);

Item xitnone();

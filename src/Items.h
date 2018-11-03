#pragma once

#include "Item.h"

typedef struct
{
    Item* item;
    int max;
}
Items;

Items i_build(const int max);

int i_add(Items, const Item);

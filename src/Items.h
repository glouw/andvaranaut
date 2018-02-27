#pragma once

#include "Item.h"

typedef struct
{
    Item* item;
    int max;
}
Items;

Items xitsnew(const int max);

int xitsadd(Items, const Item);

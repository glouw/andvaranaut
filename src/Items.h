#pragma once

#include "Item.h"

typedef struct
{
    Item* item;
    int count;
    int max;
}
Items;

Items xisnew(const int max);

Items xisadd(Items, const Item);

#pragma once

#include "Point.h"
#include "Method.h"
#include "Item.h"

typedef struct
{
    Item item;
    Point velocity;
    Point sum;
    Method method;
    int scroll_index;
    Point reticule;
}
Attack;

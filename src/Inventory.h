#pragma once

#include "Items.h"
#include "Input.h"
#include "Timer.h"

typedef struct
{
    Items items;
    int selected;
    int width;
    int hilited;
}
Inventory;

Inventory xzinv();

Inventory xinvnew();

Inventory xinvselect(Inventory, const Input);

Inventory xinvhilite(Inventory, const Input, const int xres);

void xwhatis(const Inventory, const Timer);

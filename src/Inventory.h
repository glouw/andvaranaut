#pragma once

#include "Items.h"
#include "Input.h"

typedef struct
{
    Items items;
    int selected;
    int width;
}
Inventory;

Inventory xzinv();

Inventory xinvnew();

Inventory xinvselect(Inventory inv, const Input);

void xwhatis(const Inventory, const Input, const int xres);
